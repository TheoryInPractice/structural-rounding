
#include <Python.h>

#include "bipartite.hpp"
#include "pyset.hpp"
#include "pygraph.hpp"

static PyObject* bipartite_verifybip(PyObject* self, PyObject* args) {
	PyObject* g;
	PyObject* s;
	if (!PyArg_ParseTuple(args, "OO", &g, &s)) {
		return NULL;
	}

	Graph* graph = ((PyGraph*) g)->g;
	Set* os = ((PySet*) s)->s;

	Set** od = verify_bipartite(graph, os);
	PyObject* l = make_PySet(od[1], false);
	PyObject* r = make_PySet(od[2], false);
	PyObject* o = make_PySet(od[0], false);
	// delete od;
	return Py_BuildValue("OOO", o, l, r);
}

static PyObject* bipartite_prescribed(PyObject* self, PyObject* args) {
	PyObject* bytes;
	PyObject* g;
	if (!PyArg_ParseTuple(args, "OO&", &g, PyUnicode_FSConverter, &bytes)) {
		return NULL;
	}

	char* s;
	Py_ssize_t len;
	PyBytes_AsStringAndSize(bytes, &s, &len);
	Py_DECREF(bytes);

	Graph* graph = ((PyGraph*) g)->g;

	Set* oct = prescribed_octset(graph, s);
	PyObject* o = make_PySet(oct, false);
	return o;
}

static PyObject* bipartite_vertexdelete(PyObject* self, PyObject* args) {
	PyObject* g;
	if (!PyArg_ParseTuple(args, "O", &g)) {
		return NULL;
	}

	PyGraph* graph = (PyGraph*) g;

	Set* oct = vertex_delete(graph->g);
	PyObject* o = make_PySet(oct, false);
	return o;
}

static PyMethodDef bipartite_methods[] = {
	{"verify_bipartite", bipartite_verifybip, METH_VARARGS, "computes an oct decomposition from a given octset"},
	{"prescribed_octset", bipartite_prescribed, METH_VARARGS, "reads a predetermined octset from a file"},
	{"vertex_delete", bipartite_vertexdelete, METH_VARARGS, "computes an octset for a given graph"},
	{NULL},
};

static struct PyModuleDef bipartite_module = {
	PyModuleDef_HEAD_INIT,
	"octset",
	"Python interface for oct finding operations",
	-1,
	bipartite_methods
};

PyMODINIT_FUNC PyInit_lib_bipartite() {
	return PyModule_Create(&bipartite_module);
}
