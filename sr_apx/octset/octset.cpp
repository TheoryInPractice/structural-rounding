
#include "octset.hpp"

#include <deque>
#include <iostream>
#include <fstream>

OctDecomp* verify_bip(Graph* graph, Set* os) {
	Set* left = new Set();
	Set* right = new Set();
	Set* octset = new Set();

	Set visited;
	for (Set::Iterator it = os->begin(); it != os->end(); ++it) {
		visited.insert(*it);
	}

	auto giter = graph->begin();

	std::deque<int> queue;
	int current;

	while (visited.size() < graph->size() || !queue.empty()) {
		if (queue.empty()) {
			while (visited.contains(*giter)) {
				++giter;
			}

			current = *giter;
			left->insert(current);
			visited.insert(current);
		}
		else {
			current = queue.front();
			queue.pop_front();
		}

		Set* neighbors = graph->neighbors(current);
		for (Set::Iterator it = neighbors->begin(); it != neighbors->end(); ++it) {
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

	OctDecomp* ret = new OctDecomp();
	ret->left = left;
	ret->right = right;
	ret->octset = octset;

	return ret;
}

OctDecomp* prescribed_octset(Graph* graph, const char* filename) {
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

	OctDecomp* ret = verify_bip(graph, octset);
	if (ret->octset->size() > 0) {
		printf("not a valid prescribed octset");
		return NULL;
	}

	ret->octset = octset;
	return ret;
}

void remove_vertex_oct(Graph* g, Map<int>& deg, Map<Set>& revdeg, int u) {
	for (auto nbr = g->neighbors(u)->begin(); nbr != g->neighbors(u)->end(); ++nbr) {
		if (!deg.contains(*nbr)) {
			continue;
		}

		int degree = deg[*nbr];
		revdeg[degree].erase(*nbr);
		deg[*nbr] = degree - 1;
		revdeg[degree - 1].insert(*nbr);
	}

	revdeg[deg[u]].erase(u);
	deg.erase(u);
}

Set* maximal_indset(Graph* graph, Set* available) {
	Map<int> deg;
	Map<Set> revdeg;

	for (Set::Iterator iu = available->begin(); iu != available->end(); ++iu) {
		int u = *iu;
		deg[u] = 0;
		for (Set::Iterator iv = graph->neighbors(u)->begin(); iv != graph->neighbors(u)->end(); ++iv) {
			int v = *iv;
			if (available->contains(v)) {
				deg[u] += 1;
			}
		}

		revdeg[deg[u]].insert(u);
	}

	Set* indset = new Set();

	while (deg.size() > 0) {
		int mindeg;
		for (int i = 0; i <= deg[*(deg.begin())]; ++i) {
			if (revdeg[i].size() > 0) {
				mindeg = i;
				break;
			}
		}

		int u = *(revdeg[mindeg].begin());
		for (Set::Iterator iv = graph->neighbors(u)->begin(); iv != graph->neighbors(u)->end(); ++iv) {
			int v = *iv;
			if (!deg.contains(v)) {
				continue;
			}

			remove_vertex_oct(graph, deg, revdeg, v);
		}

		revdeg[0].erase(u);
		deg.erase(u);

		indset->insert(u);
		available->erase(u);
	}

	return indset;
}

OctDecomp* find_octset(Graph* graph) {
	Set* octset = new Set();
	for (auto iu = graph->begin(); iu != graph->end(); ++iu) {
		int u = *iu;
		octset->insert(u);
	}

	Set* left = maximal_indset(graph, octset);
	Set* right = maximal_indset(graph, octset);

	OctDecomp* ret = new OctDecomp();
	ret->left = left;
	ret->right = right;
	ret->octset = octset;

	return ret;
}
