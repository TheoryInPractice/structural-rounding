
#include "vc_lift.hpp"

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
