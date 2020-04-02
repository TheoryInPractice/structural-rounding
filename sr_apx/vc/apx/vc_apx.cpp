
#include "vc_apx.hpp"
#include <vector>

Set* dfs_apx(Graph* g) {
	Set* cover = new Set();

	std::vector<int> stack;
	Set visited;

	auto vitr = g->begin();

	while (visited.size() < g->size()) {
		int current;
		if (stack.empty()) {
			while (visited.contains(*vitr)) {
				++vitr;
			}

			current = *vitr;
		}
		else {
			current = stack.back();
			stack.pop_back();
			int previous = stack.back();
			stack.pop_back();

			if (visited.contains(current)) {
				continue;
			}

			cover->insert(previous);
		}

		visited.insert(current);

		Set* nbrs = g->neighbors(current);
		for (auto it = nbrs->begin(); it != nbrs->end(); ++it) {
			if (!visited.contains(*it)) {
				stack.push_back(current);
				stack.push_back(*it);
			}
		}
	}


	return cover;
}

void remove_vertex(Graph* g, Map<int>& deg, Map<Set>& revdeg, int u) {
	for (auto nbr = g->neighbors(u)->begin(); nbr != g->neighbors(u)->end(); ++nbr) {
		if (!deg.contains(*nbr)) {
			continue;
		}

		int degree = deg[*nbr];
		revdeg[degree].erase(*nbr);

		if (degree == 1) {
			deg.erase(*nbr);
		}
		else {
			deg[*nbr] = degree - 1;
			revdeg[degree - 1].insert(*nbr);
		}
	}

	deg.erase(u);
}

Set* heuristic_apx(Graph* g) {
	Set* cover = new Set();

	Map<int> deg;
	deg.reserve(g->size());
	Map<Set> revdeg;
	int maxdeg = 0;

	for (auto it = g->begin(); it != g->end(); ++it) {
		int degree = g->degree(*it);
		deg[*it] = degree;
		revdeg[degree].insert(*it);
		maxdeg = degree > maxdeg ? degree : maxdeg;
	}

	while (deg.size() > 0) {
		while (revdeg[maxdeg].empty()) {
			--maxdeg;
		}

		int u = *(revdeg[maxdeg].begin());
		revdeg[maxdeg].erase(u);
		cover->insert(u);
		remove_vertex(g, deg, revdeg, u);
	}

	return cover;
}

Set* std_apx(Graph* g) {
	Set* cover = new Set();

	Map<int> deg;
	deg.reserve(g->size());
	Map<Set> revdeg;
	int maxdeg = 0;

	for (auto it = g->begin(); it != g->end(); ++it) {
		int degree = g->degree(*it);
		deg[*it] = degree;
		revdeg[degree].insert(*it);
		maxdeg = degree > maxdeg ? degree : maxdeg;
	}

	while (deg.size() > 0) {
		while (revdeg[maxdeg].empty()) {
			--maxdeg;
		}

		int u = *(revdeg[maxdeg].begin());
		int v;
		int md = 0;
		for (auto it = g->neighbors(u)->begin(); it != g->neighbors(u)->end(); ++it) {
			if (!deg.contains(*it)) {
				continue;
			}

			if (g->degree(*it) > md) {
				md = g->degree(*it);
				v = *it;
			}
		}
		revdeg[maxdeg].erase(u);
		cover->insert(u);
		remove_vertex(g, deg, revdeg, u);

		revdeg[deg[v]].erase(v);
		cover->insert(v);
		remove_vertex(g, deg, revdeg, v);
	}

	return cover;
}
