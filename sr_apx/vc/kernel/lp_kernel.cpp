
#include "lp_kernel.hpp"
#include "matching.hpp"

Set* lp_kernel(Graph* g) {

    int n = g->size();
    Graph* h = new Graph();
    Set* left = new Set();
    Set* right = new Set();

    for (auto iu = g->begin(); iu != g->end(); ++iu) {
        int u = *iu;

        left->insert(u);
        right->insert(u + n);

        for (auto inbr = g->neighbors(u)->begin(); inbr != g->neighbors(u)->end(); ++inbr) {
            int nbr = *inbr;
            h->add_edge(u, nbr + n);
        }
    }

    Map<int> weights;
    Map<int>* matching = bipartite_matching(h, left, right);

    for (auto im = matching->begin(); im != matching->end(); ++im) {
        int mk = *im;
        int mv = matching->at(mk);

        int u = mk >= n ? mk - n : mk;
        int v = mv >= n ? mv - n : mv;

        weights[u] = weights.contains(u) ? weights[u] + 1 : 1;
        weights[v] = weights.contains(v) ? weights[v] + 1 : 1;
    }

    Set* kernel = new Set();

    for (auto iw = weights.begin(); iw != weights.end(); ++iw) {
        int w = *iw;

        // (u, v) in matching implies (v, u) in matching,
        //  so a given matched pair will be counted twice;
        // weights[w] \in {0, 2, 4}
        if (weights[w] == 2) {
            kernel->insert(w);
        }
    }

    // delete h, left, right, matching?
    return kernel;
}
