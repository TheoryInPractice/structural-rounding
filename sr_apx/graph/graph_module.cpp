
#include <Python.h>

#include "graph.hpp"
#include "pyset.hpp"
#include "pygraph.hpp"

// graph type /////////////////////////////////////////

static void Graph_dealloc(PyGraph* self) {
	PyObject_GC_UnTrack(self);
	delete self->g;
	Py_TYPE(self)->tp_free((PyObject*) self);
}

static int Graph_traverse(PyObject* self, visitproc visit, void* arg) {
	return 0;
}

static int Graph_clear(PyObject* self) {
    return 0;
}

static PyObject* Graph_new(PyTypeObject* type, PyObject* args, PyObject* kwrds) {
	PyGraph* self;
	self = (PyGraph*) type->tp_alloc(type, 0);
	return (PyObject*) self;
}

static int Graph_init(PyGraph* self, PyObject* args) {
	int n;
	if (!PyArg_ParseTuple(args, "i", &n)) {
		self->g = new Graph();
	}
	else {
		self->g = new Graph(n);
	}

	return 0;
}

static PyObject* Graph_addedge(PyGraph* self, PyObject* args) {
	int u, v;
	if (!PyArg_ParseTuple(args, "ii", &u, &v)) {
		return NULL;
	}

	self->g->add_edge(u, v);
	Py_RETURN_NONE;
}

static PyObject* Graph_size(PyGraph* self) {
	return PyLong_FromLong(self->g->size());
}

static long Graph_size_len(PyObject* self) {
	PyGraph* temp = (PyGraph*) self;
	return temp->g->size();
}

static PyObject* Graph_degree(PyGraph* self, PyObject* args) {
	int u;
	if (!PyArg_ParseTuple(args, "i", &u)) {
		return NULL;
	}

	return PyLong_FromLong(self->g->degree(u));
}

static PyObject* Graph_adjacent(PyGraph* self, PyObject* args) {
	int u, v;
	if (!PyArg_ParseTuple(args, "ii", &u, &v)) {
		return NULL;
	}

	if (self->g->adjacent(u, v)) {
		Py_RETURN_TRUE;
	}
	else {
		Py_RETURN_FALSE;
	}
}

static PyObject* Graph_contains(PyGraph* self, PyObject* args) {
	int u;
	if (!PyArg_ParseTuple(args, "i", &u)) {
		return NULL;
	}

	if (self->g->adjlist.contains(u)) {
		Py_RETURN_TRUE;
	}
	else {
		Py_RETURN_FALSE;
	}
}

static int Graph_contains_in(PyGraph* self, PyObject* key) {
	int u = PyLong_AsLong(key);
	return self->g->adjlist.contains(u);
}

static PyObject* Graph_neighbors(PyGraph* self, PyObject* args) {
	int u;
	if (!PyArg_ParseTuple(args, "i", &u)) {
		return NULL;
	}

	return make_PySet(self->g->neighbors(u), true);
}

static PyObject* Graph_subgraph(PyGraph* self, PyObject* args) {
	PyObject* s;
	if (!PyArg_ParseTuple(args, "O", &s)) {
		return NULL;
	}

	Set* vertices = ((PySet*) s)->s;
	Graph* subg = self->g->subgraph(vertices);
	return make_PyGraph(subg);
}

static PyMethodDef Graph_methods[] = {
	{"contains", (PyCFunction) Graph_contains, METH_VARARGS, "check if a vertex is in the graph"},
	{"__contains__", (PyCFunction) Graph_contains, METH_VARARGS, "check if a vertex is in the graph"},
	{"size", (PyCFunction) Graph_size, METH_NOARGS, "gets the number of vertices in the graph"},
	{"__sizeof__", (PyCFunction) Graph_size, METH_NOARGS, "gets the number of vertices in the graph"},
	{"add_edge", (PyCFunction) Graph_addedge, METH_VARARGS, "adds an edge to the graph"},
	{"degree", (PyCFunction) Graph_degree, METH_VARARGS, "gets the degree of a vertex"},
	{"adjacent", (PyCFunction) Graph_adjacent, METH_VARARGS, "gets whether two vertices are connected by an edge"},
	{"neighbors", (PyCFunction) Graph_neighbors, METH_VARARGS, "gets the set of neighbors"},
	{"subgraph", (PyCFunction) Graph_subgraph, METH_VARARGS, "creates a subgraph containing the specified vertices"},
	{NULL},
};

static PySequenceMethods Graph_sequence_methods = {
	.sq_length = Graph_size_len,
	.sq_contains = (objobjproc) Graph_contains_in,
};

// graph iter type ////////////////////////////////////////

typedef struct {
	PyObject_HEAD
	PyGraph* g;
	Map<Set>::Iterator current;
	int len;
} PyGraphIter;

static void GraphIter_dealloc(PyGraphIter* self) {
	PyObject_GC_UnTrack(self);
	Py_XDECREF(self->g);
	PyObject_GC_Del(self);
}

static int GraphIter_traverse(PyObject* self, visitproc visit, void* arg) {
	PyGraphIter* temp = (PyGraphIter*) self;
	Py_VISIT(temp->g);
	return 0;
}

static PyObject* GraphIter_len(PyGraphIter* self) {
	int len = 0;
	if (self->g != NULL) {
		len = self->len;
	}

	return PyLong_FromLong(len);
}

static PyObject* GraphIter_iternext(PyGraphIter* self) {
	if (self->current == self->g->g->adjlist.end()) {
		PyGraph* temp = self->g;
		self->g = NULL;
		Py_DECREF(temp);
		PyErr_SetNone(PyExc_StopIteration);
		return NULL;
	}

	PyObject* ret = PyLong_FromLong(*(self->current));

	++(self->current);
	--(self->len);

	return ret;
}

static PyMethodDef GraphIter_methods[] = {
	{"__length_hint__", (PyCFunction) GraphIter_len, METH_NOARGS, "gets remaining elements in sequence"},
	{NULL},
};

PyTypeObject GraphIter_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "graph.GraphIterator",
	.tp_basicsize = sizeof(PyGraphIter),
	.tp_itemsize = 0,
	.tp_dealloc = (destructor) GraphIter_dealloc,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,
	.tp_doc = "Graph iterator object",
	.tp_traverse = GraphIter_traverse,
	.tp_iter = PyObject_SelfIter,
	.tp_iternext = (iternextfunc) GraphIter_iternext,
	.tp_methods = GraphIter_methods,
};

static PyObject* Graph_iter(PyGraph* self) {
	PyGraphIter* iter = PyObject_GC_New(PyGraphIter, &GraphIter_type);
	if (iter == NULL) {
		return NULL;
	}

	Py_INCREF(self);
	iter->g = self;
	iter->current = Map<Set>::Iterator(&(self->g->adjlist));
	iter->len = self->g->size();
	PyObject_GC_Track(iter);
	return (PyObject*) iter;
}

static PyTypeObject Graph_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "graph.Graph",
	.tp_basicsize = sizeof(PyGraph),
	.tp_itemsize = 0,
	.tp_dealloc = (destructor) Graph_dealloc,
	.tp_as_sequence = &Graph_sequence_methods,
	.tp_hash = PyObject_HashNotImplemented,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
	.tp_doc = "Graph object",
	.tp_traverse = Graph_traverse,
	.tp_iter = (getiterfunc) Graph_iter,
	.tp_methods = Graph_methods,
	.tp_init = (initproc) Graph_init,
	.tp_alloc = PyType_GenericAlloc,
	.tp_new = Graph_new,
	.tp_free = PyObject_GC_Del,
};

static PyModuleDef graph_module = {
	PyModuleDef_HEAD_INIT,
	.m_name = "graph",
	.m_doc = "module for graphs",
	.m_size = -1,
};

PyMODINIT_FUNC PyInit_graph() {
	PyObject* m;
	if (PyType_Ready(&Graph_type) < 0 || PyType_Ready(&GraphIter_type) < 0) {
		return NULL;
	}

	m = PyModule_Create(&graph_module);
	if (m == NULL) {
		return NULL;
	}


	Py_INCREF(&Graph_type);
	if (PyModule_AddObject(m, "Graph", (PyObject*) &Graph_type) < 0) {
		Py_DECREF(&Graph_type);
		Py_DECREF(m);
		return NULL;
	}

	Py_INCREF(&GraphIter_type);
	if (PyModule_AddObject(m, "GraphIterator", (PyObject*) &GraphIter_type) < 0) {
		Py_DECREF(&GraphIter_type);
		Py_DECREF(m);
		return NULL;
	}

    return m;
}

// cpp api /////////////////////////////////////

PyObject* make_PyGraph(Graph* base) {
	PyGraph* ret = (PyGraph*) Graph_new(&Graph_type, NULL, NULL);
	ret->g = base;
	return (PyObject*) ret;
}
