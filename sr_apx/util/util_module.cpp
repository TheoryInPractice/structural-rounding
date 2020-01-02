
#include <Python.h>
#include "util.hpp"

static PyObject* util_log2(PyObject* self, PyObject* args) {
	int n;
	if (!PyArg_ParseTuple(args, "i", &n)) {
		return NULL;
	}

	int x = log2(n);
	return PyLong_FromLong(x);
}

static PyObject* util_pow(PyObject* self, PyObject* args) {
	int b, e;
	if (!PyArg_ParseTuple(args, "ii", &b, &e)) {
		return NULL;
	}

	int x = pow(b, e);
	return PyLong_FromLong(x);
}

static PyMethodDef util_methods[] = {
	{"log2", util_log2, METH_VARARGS, "log base 2 integers"},
	{"pow", util_pow, METH_VARARGS, "takes b to the e"},
	{NULL},
};

static struct PyModuleDef util_module = {
	PyModuleDef_HEAD_INIT,
	"util",
	"Python interface for util methods",
	-1,
	util_methods
};

PyMODINIT_FUNC PyInit_util(void) {
	return PyModule_Create(&util_module);
}
