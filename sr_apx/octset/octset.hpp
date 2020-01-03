
#ifndef OCTSET_H
#define OCTSET_H

#include "graph.hpp"
#include "setmap.hpp"

struct OctDecomp {
	Set* left;
	Set* right;
	Set* octset;
};

OctDecomp* verify_bip(Graph*, Set*);
OctDecomp* prescribed_octset(Graph*, const char*);
OctDecomp* find_octset(Graph*);

#endif
