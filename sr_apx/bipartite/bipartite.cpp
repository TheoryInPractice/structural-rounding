
#include "bipartite.hpp"

// consider vector here
#include <deque>
#include <iostream>
#include <fstream>

Set* prescribed_octset(Graph* graph, const char* filename) {
	std::ifstream f;
	f.open(filename, std::ios::in);

	Set* octset = new Set();

	char s[100];
	while (f.getline(s, 100)) {
		int u;
		sscanf(s, "%d", &u);
		octset->insert(u);
	}

	f.close();
	return octset;
}

Set** verify_bipartite(Graph* graph, Set* os) {
	Set* left = new Set();
	Set* right = new Set();
	Set* octset = new Set();

	Set visited;
	for (Set::Iterator it = os->begin(); it != os->end(); ++it) {
		visited.insert(*it);
	}

	Set::Iterator git = graph->begin();

	std::deque<int> queue;
	int current;

	while (visited.size() < graph->size() || !queue.empty()) {
		if (queue.empty()) {
			while (visited.contains(*giter)) {
				++git;
			}

			current = *git;
			left->insert(current);
			visited.insert(current);
		}
		else {
			current = queue.front();
			queue.pop_front();
		}

		Set* nbrs = graph->neighbors(current);
		for (Set::Iterator it = nbrs->begin(); it != nbrs->end(); ++it) {
			int nbr = *it;
			if (os->contains(nbr) || octset->contains(nbr)) {
				continue;
			}

			if (left->contains(current)) {
				right->insert(nbr);
			}
			else {
				left->insert(nbr);
			}

			if (left->contains(nbr) && right->contains(nbr)) {
				left->erase(nbr);
				right->erase(nbr);
				octset->insert(nbr);
				continue;
			}

			if (!visited.contains(nbr)) {
				visited.insert(nbr);
				queue.push_back(nbr);
			}
		}
	}

	Set** ret = new Set*[3];
	ret[0] = octset;
	ret[1] = left;
	ret[2] = right;
	return ret;
}

Set* vertex_delete(Graph* graph) {
	Set* octset = new Set();
	for (auto it = graph->begin(); it != graph->end(); ++it) {
		octset->insert(*it);
	}

	remove_indset(graph, octset);
	remove_indset(graph, octset);

	return octset;
}

void remove_indset(Graph* graph, Set* available) {
	Map<int> deg;
	Map<Set> revdeg;

	for (Set::Iterator iu = available->begin(); iu != available->end(); ++iu) {
		int u = *iu;
		deg[u] = 0;
		Set* nbrs = graph->neighbors(u);
		for (Set::Iterator iv = nbrs->begin(); iv != nbrs->end(); ++iv) {
			int v = *iv;
			if (available->contains(v)) {
				deg[u] += 1;
			}
		}

		revdeg[deg[u]].insert(u);
	}

	while (deg.size() > 0) {
		int mindeg;
		for (mindeg = 0; mindeg <= deg[*(deg.begin())]; ++mindeg) {
			if (revdeg[mindeg].size() > 0) {
				break;
			}
		}

		int u = *(revdeg[mindeg].begin());
		revdeg[mindeg].erase(u);
		deg.erase(u);
		available->erase(u);

		Set* unbrs = graph->neighbors(u);
		for (Set::Iterator iv = unbrs->begin(); iv != unbrs->end(); ++iv) {
			int v = *iv;
			if (!deg.contains(v)) {
				continue;
			}

			revdeg[deg[v]].erase(v);
			deg.erase(v);

			Set* vnbrs = graph->neighbors(v);
			for (Set::Iterator iw = vnbrs->begin(); iw != vnbrs->end(); ++iw) {
				int w = *iw;
				if (!deg.contains(w)) {
					continue;
				}

				int degree = deg[w];
				revdeg[degree].erase(w);
				deg[w] = degree - 1;
				revdeg[degree - 1].insert(w);
			}
		}
	}
}
