
#include "lp_kernel.hpp"

Set* lp_kernel(Graph* g) {
    // Graph h = new Graph();
    int n = g->size();
    int source = n * 2 + 1;
    int sink = n * 2 + 2;
    Map<Set> h;

    for (auto us = g->begin(); us != g->end(); ++us) {
        int u = *us;
        for (auto vs = g->neighbors(u)->begin(); vs != g->neighbors(u)->begin(); ++vs) {
            //h.add_edge(v, (*inbr) + g->size());
            int v = *vs;
            h[u].insert(v + n);
        }
        h[source].insert(u);
        h[u + n].insert(sink);
    }

    return NULL;
}