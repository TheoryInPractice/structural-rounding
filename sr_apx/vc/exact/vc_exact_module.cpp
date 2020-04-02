
#include <Python.h>

#include "vc_exact.hpp"
#include "pygraph.hpp"
#include "pyset.hpp"
#include "graph.hpp"

static PyObject* vc_exact_bipexact(PyObject* self, PyObject* args) {
	PyObject* g;
	if (!PyArg_ParseTuple(args, "O", &g)) {
		return NULL;
	}

	Graph* graph = ((PyGraph*) g)->g;
	return make_PySet(bip_exact(graph), false);
}

static PyMethodDef vc_exact_methods[] = {
	{"bip_exact", vc_exact_bipexact, METH_VARARGS, "computes a minimum vertex cover in a bipartite graph"},
	{NULL},
};

static struct PyModuleDef vc_exact_module = {
	PyModuleDef_HEAD_INIT,
	"vc_exact",
	"Python interface for vertex cover algorithms",
	-1,
	vc_exact_methods
};

PyMODINIT_FUNC PyInit_lib_vc_exact() {
	return PyModule_Create(&vc_exact_module);
}
