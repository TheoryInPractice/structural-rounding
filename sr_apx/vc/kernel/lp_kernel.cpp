
#include "lp_kernel.hpp"

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
    Map<int>* matching = maximum_matching(h, left, right);

    Set* kernel = new Set();

    for (auto im = matching->begin(); im != matching->end(); ++im) {
        int key = *im > n - 1 ? *im - n : *im;
        int value = matching->at(key) > n - 1 ? matching->at(key) - n : matching->at(key);

        if (!weights.contains(key)) {
            weights[key] = 1;
        }
        else {
            weights[key]++;
        }

        if (!weights.contains(value)) {
            weights[value] = 1;
        }
        else {
            weights[value]++;
        }
    }
    
    for (auto iw = weights.begin(); iw != weights.end(); ++iw) {
        int w = *iw;

        // This assumes (u, v) in matching -> (v, u) not in matching
        // otherwise, this should be modified to weights[w] == 2
        if (weights[w] == 1) {
            kernel->insert(w);
        }
    }

    // delete h, left, right, matching?

    return kernel;
}
