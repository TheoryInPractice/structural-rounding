
#include <Python.h>

#include "octset.hpp"
#include "pyset.hpp"
#include "pygraph.hpp"

static PyObject* octset_verifybip(PyObject* self, PyObject* args) {
	PyObject* g;
	PyObject* s;
	if (!PyArg_ParseTuple(args, "OO", &g, &s)) {
		return NULL;
	}

	Graph* graph = ((PyGraph*) g)->g;
	Set* os = ((PySet*) s)->s;

	OctDecomp* od = verify_bip(graph, os);
	PyObject* l = make_PySet(od->left, false);
	PyObject* r = make_PySet(od->right, false);
	PyObject* o = make_PySet(od->octset, false);
	return Py_BuildValue("OOO", l, r, o);
}

static PyObject* octset_prescribed(PyObject* self, PyObject* args) {
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

	OctDecomp* od = prescribed_octset(graph, s);
	PyObject* l = make_PySet(od->left, false);
	PyObject* r = make_PySet(od->right, false);
	PyObject* o = make_PySet(od->octset, false);
	return Py_BuildValue("OOO", l, r, o);
}

static PyObject* octset_findoctset(PyObject* self, PyObject* args) {
	PyObject* g;
	if (!PyArg_ParseTuple(args, "O", &g)) {
		return NULL;
	}

	PyGraph* graph = (PyGraph*) g;

	OctDecomp* od = find_octset(graph->g);
	PyObject* l = make_PySet(od->left, false);
	PyObject* r = make_PySet(od->right, false);
	PyObject* o = make_PySet(od->octset, false);
	return Py_BuildValue("OOO", l, r, o);
}

static PyMethodDef octset_methods[] = {
	{"verify_bip", octset_verifybip, METH_VARARGS, "computes an oct decomposition from a given octset"},
	{"prescribed_octset", octset_prescribed, METH_VARARGS, "reads a predetermined octset from a file"},
	{"find_octset", octset_findoctset, METH_VARARGS, "computes an octset for a given graph"},
	{NULL},
};

static struct PyModuleDef octset_module = {
	PyModuleDef_HEAD_INIT,
	"octset",
	"Python interface for oct finding operations",
	-1,
	octset_methods
};

PyMODINIT_FUNC PyInit_octset() {
	return PyModule_Create(&octset_module);
}
