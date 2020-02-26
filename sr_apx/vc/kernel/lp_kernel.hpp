
#ifndef LP_KERNEL_H
#define LP_KERNEL_H

#include "setmap.hpp"
#include "graph.hpp"

Set* lp_kernel(Graph*);

extern Map<int>* maximum_matching(Graph*, Set*, Set*);

#endif
