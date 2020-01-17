
#include "graph.hpp"
#include "graphio.hpp"
#include "setmap.hpp"

#include <cstdio>

int main() {
	Graph* g = read_sparse6("graphs/small/a.s6");
	printf("%d\n", g->size());
}
