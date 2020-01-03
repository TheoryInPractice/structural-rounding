
#include "util.hpp"
#include "graphio.hpp"

#include <iostream>
#include <fstream>

#define BUFFER_SIZE 1024

Graph* read_sparse6(const char* filename) {
	std::ifstream f;
	f.open(filename, std::ios::in | std::ios::binary);
	char c[7];
	f.read(c, 1);
	if (c[0] != ':') {
		printf("%s\n", "not sparse6");
		return NULL;
	}

	int n;
	f.read(c, 1);
	if (c[0] - 63 < 63) {
		n = c[0] - 63;
	}
	else {
		f.read(c, 3);
		if (c[0] - 63 < 63) {
			n = (c[0] - 63) << 12;
			n += (c[1] - 63) << 6;
			n += (c[2] - 63);
		}
		else {
			f.read(&c[3], 4);
			n = (c[1] - 63) << 30;
			n += (c[2] - 63) << 24;
			n += (c[3] - 63) << 18;
			n += (c[4] - 63) << 12;
			n += (c[5] - 63) << 6;
			n += (c[6] - 63);
		}
	}

	int k = log2(n);
	Graph* graph = new Graph(n);

	int bitbuffer = 0;
	int bitavailable = 0;

	int position = 0;
	int available = 0;
	char buffer[BUFFER_SIZE];

	int v = 0;
	while (position < available || !f.eof()) {
		if (bitavailable < 1) {
			if (position == available) {
				f.read(buffer, BUFFER_SIZE);
				available = f.gcount();
				position = 0;
			}

			bitbuffer = buffer[position++] - 63;
			bitavailable = 6;
		}
		bitavailable -= 1;
		int b = bitbuffer >> bitavailable;
		bitbuffer &= (1 << bitavailable) - 1;

		while (bitavailable < k) {
			if (position == available) {
				f.read(buffer, BUFFER_SIZE);
				available = f.gcount();
				position = 0;
			}

			bitbuffer = (bitbuffer << 6) + (buffer[position++] - 63);
			bitavailable += 6;
		}

		bitavailable -= k;
		int x = bitbuffer >> bitavailable;
		bitbuffer &= (1 << bitavailable) - 1;

		if (b == 1) {
			v += 1;
		}

		if (x >= n || v >= n) {
			break;
		}
		else if (x > v) {
			v = x;
		}
		else {
			graph->add_edge(x, v);
		}
	}

	f.close();
	return graph;
}

Graph* read_edge_list(const char* filename) {
	std::ifstream f;
	f.open(filename, std::ios::in);

	Graph* g = new Graph();

	char s[100];
	while (f.getline(s, 100)) {
		int u, v;
		sscanf(s, "%d %d", &u, &v);
		g->add_edge(u, v);
	}

	f.close();
	return g;
}
