
#ifndef PYGRAPH_H
#define PYGRAPH_H

#include <Python.h>
#include "graph.hpp"

PyObject* make_PyGraph(Graph*);

#endif
