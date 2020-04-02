
#include "vc_exact.hpp"
#include "bipartite.hpp"
#include "matching.hpp"

#include <cstdio>
#include <vector>

Set* bip_exact(Graph* graph) {
	Set* empty = new Set();
	Set** od = verify_bipartite(graph, empty);
	delete empty;

	if (od[0]->size() > 0) {
		printf("%s\n", "not a bipartite graph");
		return NULL;
	}

	Set* left = od[1];
	Set* right = od[2];
	delete[] od;

	Map<int>* match = bipartite_matching(graph, left, right);

	Set* cover = new Set();
	Set visited;
	std::vector<int> stack;

	for (Set::Iterator iu = left->begin(); iu != left->end(); ++iu) {
		int u = *iu;
		cover->insert(u);
		if (!match->contains(u)) {
			visited.insert(u);
			stack.push_back(u);
		}
	}

	int current;
	while (!stack.empty()) {
		current = stack.back();
		stack.pop_back();

		cover->erase(current);

		for (Set::Iterator inbr = graph->neighbors(current)->begin(); inbr != graph->neighbors(current)->end(); ++inbr) {
			int nbr = *inbr;

			if (!match->contains(nbr)) {
				cover->insert(nbr);
				continue;
			}

			if (match->at(nbr) != current) {
				cover->insert(nbr);
			}

			if (!visited.contains(match->at(nbr))) {
				visited.insert(match->at(nbr));
				stack.push_back(match->at(nbr));
			}
		}
	}

	delete match;

	return cover;
}
