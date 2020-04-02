
#include "vc_lift.hpp"
#include "vc_apx.hpp"
#include "vc_exact.hpp"
#include "bipartite.hpp"

Set* naive_lift(Graph* graph, Set* octset, Set* partial) {
	Set* cover = new Set();
	for (Set::Iterator iu = octset->begin(); iu != octset->end(); ++iu) {
		int u = *iu;
		cover->insert(u);
	}

	for (Set::Iterator iu = partial->begin(); iu != partial->end(); ++iu) {
		int u = *iu;
		cover->insert(u);
	}

	return cover;
}

Set* greedy_lift(Graph* graph, Set* octset, Set* partial) {
	Set* cover = new Set();
	for (Set::Iterator iu = partial->begin(); iu != partial->end(); ++iu) {
		int u = *iu;
		cover->insert(u);
	}

	Set processed;
	for (auto iu = graph->begin(); iu != graph->end(); ++iu) {
		int u = *iu;
		if (!octset->contains(u)) {
			processed.insert(u);
		}
	}

	for (Set::Iterator iu = octset->begin(); iu != octset->end(); ++iu) {
		int u = *iu;
		for (Set::Iterator iv = graph->neighbors(u)->begin(); iv != graph->neighbors(u)->end(); ++iv) {
			int v = *iv;
			if (processed.contains(v) && !cover->contains(v)) {
				cover->insert(u);
				break;
			}
		}

		processed.insert(u);
	}

	return cover;
}

Set* apx_lift(Graph* graph, Set* octset, Set* partial) {
	Set* subgraph_vertices = new Set();
	for (auto it = graph->begin(); it != graph->end(); ++it) {
		if (octset->contains(*it) || !partial->contains(*it)) {
			subgraph_vertices->insert(*it);
		}
	}

	Graph* h = graph->subgraph(subgraph_vertices);
	delete subgraph_vertices;

	Set* cover = std_apx(h);
	for (Set::Iterator it = partial->begin(); it != partial->end(); ++it) {
		cover->insert(*it);
	}

	delete h;
	return cover;
}

Set* oct_lift(Graph* graph, Set* octset, Set* partial) {
	Graph* h = graph->subgraph(octset);
	Set* cover = std_apx(h);
	for (Set::Iterator it = partial->begin(); it != partial->end(); ++it) {
		cover->insert(*it);
	}

	delete h;

	Set* subgraph_vertices = new Set();
	for (auto it = graph->begin(); it != graph->end(); ++it) {
		if (!cover->contains(*it)) {
			subgraph_vertices->insert(*it);
		}
	}

	h = graph->subgraph(subgraph_vertices);
	Set* bipcover = bip_exact(h);
	for (Set::Iterator it = bipcover->begin(); it != bipcover->end(); ++it) {
		cover->insert(*it);
	}

	delete h;
	delete subgraph_vertices;
	delete bipcover;

	return cover;
}

Set* bip_lift(Graph* graph, Set* octset, Set* partial) {
	Graph* h = new Graph();
	for (auto iu = graph->begin(); iu != graph->end(); ++iu) {
		int u = *iu;
		if (!octset->contains(u)) {
			continue;
		}

		Set* nbrs = graph->neighbors(u);
		for (Set::Iterator iv = nbrs->begin(); iv != nbrs->end(); ++iv) {
			int v = *iv;
			if (!octset->contains(v) && !partial->contains(v)) {
				h->add_edge(u, v);
			}
		}
	}

	Set* cover = bip_exact(h);
	delete h;

	Set* subgraph_vertices = new Set();
	for (Set::Iterator it = octset->begin(); it != octset->end(); ++it) {
		if (!cover->contains(*it)) {
			subgraph_vertices->insert(*it);
		}
	}

	h = graph->subgraph(subgraph_vertices);
	delete subgraph_vertices;
	Set* octcover = std_apx(h);
	delete h;

	for (Set::Iterator it = partial->begin(); it != partial->end(); ++it) {
		cover->insert(*it);
	}

	for (Set::Iterator it = octcover->begin(); it != octcover->end(); ++it) {
		cover->insert(*it);
	}

	delete octcover;

	return cover;
}

Set* recursive_lift(Graph* graph, Set* octset, Set* partial) {
	Set* subgraph_vertices = new Set();
	for (auto it = graph->begin(); it != graph->end(); ++it) {
		if (octset->contains(*it) || !partial->contains(*it)) {
			subgraph_vertices->insert(*it);
		}
	}

	Graph* h = graph->subgraph(subgraph_vertices);
	delete subgraph_vertices;

	Set* octset2 = vertex_delete(h);
	subgraph_vertices = new Set();
	for (auto it = h->begin(); it != h->end(); ++it) {
		if (!octset2->contains(*it)) {
			subgraph_vertices->insert(*it);
		}
	}

	Graph* g = h->subgraph(subgraph_vertices);
	delete h;
	delete subgraph_vertices;
	Set* cover = bip_exact(g);

	for (Set::Iterator it = partial->begin(); it != partial->end(); ++it) {
		cover->insert(*it);
	}

	for (Set::Iterator it = octset2->begin(); it != octset2->end(); ++it) {
		cover->insert(*it);
	}

	delete octset2;
	delete g;

	return cover;
}

Set* recursive_oct_lift(Graph* graph, Set* octset, Set* partial) {
	Graph* h = graph->subgraph(octset);
	Set* octset2 = vertex_delete(h);
	Set* subgraph_vertices = new Set();
	for (auto it = h->begin(); it != h->end(); ++it) {
		if (!octset2->contains(*it)) {
			subgraph_vertices->insert(*it);
		}
	}

	Graph* g = h->subgraph(subgraph_vertices);
	delete h;
	delete subgraph_vertices;
	Set* cover = bip_exact(g);
	delete g;

	for (Set::Iterator it = octset2->begin(); it != octset2->end(); ++it) {
		cover->insert(*it);
	}

	delete octset2;

	for (Set::Iterator it = partial->begin(); it != partial->end(); ++it) {
		cover->insert(*it);
	}

	subgraph_vertices = new Set();
	for (auto it = graph->begin(); it != graph->end(); ++it) {
		if (!cover->contains(*it)) {
			subgraph_vertices->insert(*it);
		}
	}

	g = graph->subgraph(subgraph_vertices);
	delete subgraph_vertices;
	Set* bipcover = bip_exact(g);
	delete g;

	for (Set::Iterator it = bipcover->begin(); it != bipcover->end(); ++it) {
		cover->insert(*it);
	}

	delete bipcover;

	return cover;
}

Set* recursive_bip_lift(Graph* graph, Set* octset, Set* partial) {
	Graph* h = new Graph();
	for (auto iu = graph->begin(); iu != graph->end(); ++iu) {
		int u = *iu;
		if (!octset->contains(u)) {
			continue;
		}

		Set* nbrs = graph->neighbors(u);
		for (Set::Iterator iv = nbrs->begin(); iv != nbrs->end(); ++iv) {
			int v = *iv;
			if (!octset->contains(v) && !partial->contains(v)) {
				h->add_edge(u, v);
			}
		}
	}

	Set* cover = bip_exact(h);
	delete h;

	Set* subgraph_vertices = new Set();
	for (Set::Iterator it = octset->begin(); it != octset->end(); ++it) {
		if (!cover->contains(*it)) {
			subgraph_vertices->insert(*it);
		}
	}

	h = graph->subgraph(subgraph_vertices);
	delete subgraph_vertices;

	Set* octset2 = vertex_delete(h);
	subgraph_vertices = new Set();
	for (auto it = h->begin(); it != h->end(); ++it) {
		if (!octset2->contains(*it)) {
			subgraph_vertices->insert(*it);
		}
	}

	Graph* g = h->subgraph(subgraph_vertices);
	delete subgraph_vertices;
	delete h;
	Set* octcover = bip_exact(g);
	delete g;

	for (Set::Iterator it = octcover->begin(); it != octcover->end(); ++it) {
		cover->insert(*it);
	}

	delete octcover;

	for (Set::Iterator it = octset2->begin(); it != octset2->end(); ++it) {
		cover->insert(*it);
	}

	delete octset2;

	for (Set::Iterator it = partial->begin(); it != partial->end(); ++it) {
		cover->insert(*it);
	}

	return cover;
}
