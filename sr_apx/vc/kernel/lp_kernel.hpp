
#ifndef LP_KERNEL_H
#define LP_KERNEL_H

#include "setmap.hpp"
#include "graph.hpp"

Set* lp_kernel(Graph*);

// not sure if this is the best way to access this fucntion from vc_exact.cpp
extern Map<int>* maximum_matching(Graph*, Set*, Set*);

#endif