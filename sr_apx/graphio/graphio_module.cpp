
#include <Python.h>

#include "graphio.hpp"
#include "pygraph.hpp"

static PyObject* graphio_read_sparse6(PyObject* self, PyObject* args) {
	PyObject* bytes;
	if (!PyArg_ParseTuple(args, "O&", PyUnicode_FSConverter, &bytes)) {
		return NULL;
	}

	char* s;
	Py_ssize_t len;
	PyBytes_AsStringAndSize(bytes, &s, &len);
	PyObject* g = make_PyGraph(read_sparse6(s));
	Py_DECREF(bytes);
	return g;
}

static PyObject* graphio_read_edge_list(PyObject* self, PyObject* args) {
	PyObject* bytes;
	if (!PyArg_ParseTuple(args, "O&", PyUnicode_FSConverter, &bytes)) {
		return NULL;
	}

	char* s;
	Py_ssize_t len;
	PyBytes_AsStringAndSize(bytes, &s, &len);
	PyObject* g = make_PyGraph(read_edge_list(s));
	Py_DECREF(bytes);
	return g;
}

static PyMethodDef graphio_methods[] = {
	{"read_sparse6", graphio_read_sparse6, METH_VARARGS, "reads a graph from file formatted as sparse6"},
	{"read_edge_list", graphio_read_edge_list, METH_VARARGS, "reads a graph from file formatted as an edge list"},
	{NULL},
};

static struct PyModuleDef graphio_module = {
	PyModuleDef_HEAD_INIT,
	"graphio",
	"Python interface for graph io operations",
	-1,
	graphio_methods
};

PyMODINIT_FUNC PyInit_lib_graphio() {
	return PyModule_Create(&graphio_module);
}
