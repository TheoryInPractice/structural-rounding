
#ifndef VC_LIFT_H
#define VC_LIFT_H

#include "graph.hpp"
#include "setmap.hpp"

Set* naive_lift(Graph*, Set*, Set*);
Set* greedy_lift(Graph*, Set*, Set*);

#endif
