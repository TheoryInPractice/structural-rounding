
#ifndef VC_LIFT_H
#define VC_LIFT_H

#include "graph.hpp"
#include "setmap.hpp"

Set* naive_lift(Graph*, Set*, Set*);
Set* greedy_lift(Graph*, Set*, Set*);
Set* apx_lift(Graph*, Set*, Set*);
Set* oct_lift(Graph*, Set*, Set*);
Set* bip_lift(Graph*, Set*, Set*);
Set* recursive_lift(Graph*, Set*, Set*);
Set* recursive_oct_lift(Graph*, Set*, Set*);
Set* recursive_bip_lift(Graph*, Set*, Set*);

#endif
