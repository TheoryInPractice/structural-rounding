
#include <Python.h>

#include "vc_apx.hpp"
#include "graph.hpp"
#include "setmap.hpp"
#include "pyset.hpp"
#include "pygraph.hpp"

static PyObject* vc_apx_dfsapx(PyObject* self, PyObject* args) {
	PyObject* graph;
	if (!PyArg_ParseTuple(args, "O", &graph)) {
		return NULL;
	}

	Graph* g = ((PyGraph*) graph)->g;
	Set* cover = dfs_apx(g);
	return make_PySet(cover, false);
}

static PyObject* vc_apx_stdapx(PyObject* self, PyObject* args) {
	PyObject* graph;
	if (!PyArg_ParseTuple(args, "O", &graph)) {
		return NULL;
	}

	Graph* g = ((PyGraph*) graph)->g;
	Set* cover = std_apx(g);
	return make_PySet(cover, false);
}

static PyObject* vc_apx_heuristic(PyObject* self, PyObject* args) {
	PyObject* graph;
	if (!PyArg_ParseTuple(args, "O", &graph)) {
		return NULL;
	}

	Graph* g = ((PyGraph*) graph)->g;
	Set* cover = heuristic_apx(g);
	return make_PySet(cover, false);
}

static PyMethodDef vc_apx_methods[] = {
	{"dfs_apx", vc_apx_dfsapx, METH_VARARGS, "uses a dfs tree to approximate a vertex cover"},
	{"std_apx", vc_apx_stdapx, METH_VARARGS, "uses the standard greedy approach to approximate a vertex cover"},
	{"heuristic_apx", vc_apx_heuristic, METH_VARARGS, "repeatedly adds the vertex of largest degree to find a vertex cover"},
	{NULL},
};

static struct PyModuleDef vc_apx_module = {
	PyModuleDef_HEAD_INIT,
	"vc_apx",
	"Python interface for vertex cover approximations",
	-1,
	vc_apx_methods
};

PyMODINIT_FUNC PyInit_lib_vc_apx() {
	return PyModule_Create(&vc_apx_module);
}
