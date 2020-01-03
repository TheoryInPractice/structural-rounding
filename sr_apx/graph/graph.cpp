
#include "graph.hpp"

Graph::Graph(int n) {
	adjlist.reserve(n);
}

bool Graph::adjacent(int u, int v) {
    return adjlist[u].contains(v);
}

void Graph::add_edge(int u, int v) {
	adjlist[u].insert(v);
	adjlist[v].insert(u);
}

int Graph::size() {
	return adjlist.size();
}

Map<Set>::Iterator Graph::begin() {
	return adjlist.begin();
}

Map<Set>::Iterator Graph::end() {
	return adjlist.end();
}

Set* Graph::neighbors(int u) {
	if (!adjlist.contains(u)) {
		return NULL;
	}
	return &(adjlist[u]);
}

int Graph::degree(int u) {
	if (!adjlist.contains(u)) {
		return 0;
	}
	return adjlist[u].size();
}

Graph* Graph::subgraph(Set* vertices) {
	Graph* subg = new Graph(vertices->size());

	for (Set::Iterator iu = vertices->begin(); iu != vertices->end(); ++iu) {
		int u = *iu;
		for (Set::Iterator iv = adjlist[u].begin(); iv != adjlist[u].end(); ++iv) {
			int v = *iv;
			if (vertices->contains(v)) {
				subg->add_edge(u, v);
			}
		}
	}

	return subg;
}
