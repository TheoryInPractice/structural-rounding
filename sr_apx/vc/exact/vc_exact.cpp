
#include "vc_exact.hpp"
#include "bipartite.hpp"

#include <cstdio>
#include <vector>
#include <deque>

Map<int>* maximum_matching(Graph* graph, Set* left, Set* right) {
	Map<int>* match = new Map<int>();

	bool update = true;

	while (update) {
		update = false;

		std::deque<int> queue;
		Map<int> distance;
		Set unmatched;

		for (Set::Iterator iu = left->begin(); iu != left->end(); ++iu) {
			int u = *iu;
			if (!match->contains(u)) {
				distance[u] = 0;
				queue.push_back(u);
				unmatched.insert(u);
			}
		}

		while (!queue.empty()) {
			int current = queue.front();
			queue.pop_front();

			for (Set::Iterator inbr = graph->neighbors(current)->begin(); inbr != graph->neighbors(current)->end(); ++inbr) {
				int nbr = *inbr;
				if (!match->contains(nbr)) {
					continue;
				}

				if (!distance.contains(match->at(nbr))) {
					distance[match->at(nbr)] = distance[current] + 1;
					queue.push_back(match->at(nbr));
				}
			}
		}

		std::vector<int> stack;
		Map<int> parent;

		while (unmatched.size() > 0 || !stack.empty()) {
			if (stack.empty()) {
				stack.push_back(-1);
				int u = *(unmatched.begin());
				unmatched.erase(u);
				stack.push_back(u);
			}

			int current = stack.back();
			stack.pop_back();
			int previous = stack.back();
			stack.pop_back();

			parent[current] = previous;
			for (Set::Iterator inbr = graph->neighbors(current)->begin(); inbr != graph->neighbors(current)->end(); ++inbr) {
				int nbr = *inbr;
				if (!match->contains(nbr)) {
					update = true;

					stack.clear();
					int c = current;
					int n = nbr;

					while (c != -1) {
						match->at(n) = c;
						int temp = match->at(c);
						match->at(c) = n;
						n = temp;
						c = parent[c];
					}

					break;
				}

				if (distance[match->at(nbr)] == distance[current] + 1) {
					stack.push_back(current);
					stack.push_back(match->at(nbr));
				}
			}

			distance.erase(current);
		}
	}

	return match;
}

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
	delte od;

	Map<int>* match = maximum_matching(graph, left, right);

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

	return cover;
}
