
#ifndef BIPARTITE_H
#define BIPARTITE_H

#include "graph.hpp"
#include "setmap.hpp"

Set* vertex_delete(Graph*);
Set* prescribed_octset(Graph*, const char*);
Set** verify_bipartite(Graph*, Set*);

#endif
