
#ifndef GRAPH_H
#define GRAPH_H

#include "setmap.hpp"

class Graph {
public:
	Map<Set> adjlist;

	Graph() {};
	Graph(int);
	~Graph();
	void add_edge(int, int);
	int size();
	int degree(int);
	bool adjacent(int, int);

	Map<Set>::Iterator begin();
	Map<Set>::Iterator end();
	Set* neighbors(int);

	Graph* subgraph(Set*);
};

Graph* read_sparse6(const char*);
Graph* read_edge_list(const char*);

#endif
