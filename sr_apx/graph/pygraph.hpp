
#ifndef PYGRAPH_H
#define PYGRAPH_H

#include <Python.h>
#include "graph.hpp"

typedef struct {
	PyObject_HEAD
	Graph* g;
} PyGraph;

PyObject* make_PyGraph(Graph*);

#endif
