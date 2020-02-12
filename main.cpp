
#include <dirent.h>
//#include <fstream>
#include <iostream>
#include <vector>

#include "time.h"

#include "graph.hpp"
#include "octset.hpp"
#include "setmap.hpp"

#include "vc_apx.hpp"
#include "vc_exact.hpp"
#include "vc_lift.hpp"

// helper function declarations ////////////////////////////////////////////////

double sum(double* vals, int len);
int min(int* vals, int len);
int max(int* vals, int len);
void read_directory(const std::string& name, std::vector<std::string>& v);

// main.py functions ///////////////////////////////////////////////////////////

double run_apx(Set* (*apx)(Graph*), Graph* graph, int n, int &minsol, int &maxsol) {
	double* times = new double[n];
	int* sols = new int[n];
	for (int i = 0; i < n; i++) {
		clock_t start = clock();
		Set* cover = apx(graph);
		clock_t end = clock();
		times[i] = (double) (end - start);
		sols[i] = cover->size();
	}

	double avgtime = sum(times, n) / n;
	minsol = min(sols, n);
	maxsol = max(sols, n);
	return avgtime;
}

double run_lift(Set* (*lift)(Graph*, Set*, Set*), Graph* graph, int n, Set* octset,
				Set* partial, int &minsol, int &maxsol) {
	double* times = new double[n];
	int* sols = new int[n];
	for (int i = 0; i < n; i++) {
		clock_t start = clock();
		Set* cover = lift(graph, octset, partial);
		clock_t end = clock();
		times[i] = (double) end - start;
		sols[i] = cover->size();
	}

	double avgtime = sum(times, n) / n;
	minsol = min(sols, n);
	maxsol = max(sols, n);
	return avgtime;
}

int main(int argc, char* argv[]) {
	std::string filepath = argv[1];
	bool directory = true;
	if (filepath[filepath.size()-1] != '/') {
		if (filepath[filepath.size()-3] == '.' && filepath[filepath.size()-2] == 's'
		    && filepath[filepath.size()-1] == '6') {
				directory = false;
		}
		if (directory) {
			printf("%s %s", filepath.c_str(), "is not a valid filename or directory\n");
			return -1;
		}
		/* check here if file exists */
	}
	int n = 1;

	/* results.csv currently doesn't get written to */
	// std::ofstream results_file;
	// results_file.open("results/results.csv");
	// results_file.close();

	std::vector<std::string> graph_files;
	if (directory)
    	read_directory(filepath, graph_files);
	else {
		if (filepath.find("/") == std::string::npos) {
			graph_files.push_back(filepath);
			filepath = "";
		}
		else {
			graph_files.push_back(filepath.substr(filepath.rfind("/")+1, filepath.length()-2));
			filepath = filepath.substr(0, filepath.rfind("/")+1);
		}
	}

    for (std::vector<std::string>::iterator graph_files_it = graph_files.begin();
		graph_files_it != graph_files.end(); graph_files_it++) {
		std::string filename = *graph_files_it;
		if (filename.find(".s6") == std::string::npos)
			continue;

		printf("%s\n", (filename.substr(0, filename.length()-3)).c_str());

		clock_t start = clock();
		Graph* graph = read_sparse6((filepath + filename).c_str());
		clock_t end = clock();
		printf("n: %d\n", graph->size());
		printf("time: %.4f\n", (double)(end-start)/1000000);

		int minsol;
		int maxsol;
		double t;

		t = run_apx(heuristic_apx, graph, n, minsol, maxsol);
		printf("heuristic apx\n");
		printf("\tavg time: %.4f\n", t/1000000);
		printf("\tmin size: %d\n", minsol);
		printf("\tmax size: %d\n", maxsol);

		t = run_apx(dfs_apx, graph, n, minsol, maxsol);
		printf("dfs apx\n");
		printf("\tavg time: %.4f\n", t/1000000);
		printf("\tmin size: %d\n", minsol);
		printf("\tmax size: %d\n", maxsol);

		t = run_apx(std_apx, graph, n, minsol, maxsol);
		printf("std apx\n");
		printf("\tavg time: %.4f\n", t/1000000);
		printf("\tmin size: %d\n", minsol);

		start = clock();
		OctDecomp* oct = find_octset(graph);

		Set* bippart = new Set();
		for (Set::Iterator left_it = oct->left->begin(); left_it != oct->left->end(); left_it++) {
			int left = *left_it;
			bippart->insert(left);
		}
		for (Set::Iterator right_it = oct->right->begin(); right_it != oct->right->end(); right_it++) {
			int right = *right_it;
			bippart->insert(right);
		}

		Set* partial = bip_exact(graph->subgraph(bippart));
		end = clock();

		printf("bip solve\n");
		printf("\tavg time: %.4f\n", (double)(end-start)/1000000);

		printf("%d\n", partial->size());

		t = run_lift(naive_lift, graph, n, oct->octset, partial, minsol, maxsol);
		printf("naive lift\n");
		printf("\tavg time: %.4f\n", t/1000000);
		printf("\tmin size: %d\n", minsol);
		printf("\tmax size: %d\n", maxsol);

		t = run_lift(greedy_lift, graph, n, oct->octset, partial, minsol, maxsol);
		printf("greedy lift\n");
		printf("\tavg time: %.4f\n", t/1000000);
		printf("\tmin size: %d\n", minsol);
		printf("\tmax size: %d\n", maxsol);

		graph = NULL;

		printf("\n");
	}

	return 0;
}

// original helper functions ///////////////////////////////////////////////////

double sum(double* vals, int len) {
	double sum = 0.0;
	for (int i = 0; i < len; i++)
		sum += vals[i];
	return sum;
}

int min(int* vals, int len) {
	int min = vals[0];
	for (int i = 1; i < len; i++)
		if (vals[i] < min)
			min = vals[i];
	return min;
}

int max(int* vals, int len) {
	int max = vals[0];
	for (int i = 1; i < len; i++)
		if (vals[i] < max)
			max = vals[i];
	return max;
}

// borrowed helper functions ///////////////////////////////////////////////////

/* http://www.martinbroadhurst.com/list-the-files-in-a-directory-in-c.html */
void read_directory(const std::string& name, std::vector<std::string>& v) {
    DIR* dirp = opendir(name.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
        v.push_back(dp->d_name);
    }
    closedir(dirp);
}
