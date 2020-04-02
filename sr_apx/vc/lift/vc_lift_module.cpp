
#include <Python.h>

#include "vc_lift.hpp"
#include "pygraph.hpp"
#include "pyset.hpp"

static PyObject* vc_lift_naivelift(PyObject* self, PyObject* args) {
	PyObject* g;
	PyObject* o;
	PyObject* p;

	if (!PyArg_ParseTuple(args, "OOO", &g, &o, &p)) {
		return NULL;
	}

	Graph* graph = ((PyGraph*) g)->g;
	Set* octset = ((PySet*) o)->s;
	Set* partial = ((PySet*) p)->s;

	return make_PySet(naive_lift(graph, octset, partial), false);
}

static PyObject* vc_lift_greedylift(PyObject* self, PyObject* args) {
	PyObject* g;
	PyObject* o;
	PyObject* p;

	if (!PyArg_ParseTuple(args, "OOO", &g, &o, &p)) {
		return NULL;
	}

	Graph* graph = ((PyGraph*) g)->g;
	Set* octset = ((PySet*) o)->s;
	Set* partial = ((PySet*) p)->s;

	return make_PySet(greedy_lift(graph, octset, partial), false);
}

static PyObject* vc_lift_apxlift(PyObject* self, PyObject* args) {
	PyObject* g;
	PyObject* o;
	PyObject* p;

	if (!PyArg_ParseTuple(args, "OOO", &g, &o, &p)) {
		return NULL;
	}

	Graph* graph = ((PyGraph*) g)->g;
	Set* octset = ((PySet*) o)->s;
	Set* partial = ((PySet*) p)->s;

	return make_PySet(apx_lift(graph, octset, partial), false);
}

static PyObject* vc_lift_octlift(PyObject* self, PyObject* args) {
	PyObject* g;
	PyObject* o;
	PyObject* p;

	if (!PyArg_ParseTuple(args, "OOO", &g, &o, &p)) {
		return NULL;
	}

	Graph* graph = ((PyGraph*) g)->g;
	Set* octset = ((PySet*) o)->s;
	Set* partial = ((PySet*) p)->s;

	return make_PySet(oct_lift(graph, octset, partial), false);
}

static PyObject* vc_lift_biplift(PyObject* self, PyObject* args) {
	PyObject* g;
	PyObject* o;
	PyObject* p;

	if (!PyArg_ParseTuple(args, "OOO", &g, &o, &p)) {
		return NULL;
	}

	Graph* graph = ((PyGraph*) g)->g;
	Set* octset = ((PySet*) o)->s;
	Set* partial = ((PySet*) p)->s;

	return make_PySet(bip_lift(graph, octset, partial), false);
}

static PyObject* vc_lift_recursivelift(PyObject* self, PyObject* args) {
	PyObject* g;
	PyObject* o;
	PyObject* p;

	if (!PyArg_ParseTuple(args, "OOO", &g, &o, &p)) {
		return NULL;
	}

	Graph* graph = ((PyGraph*) g)->g;
	Set* octset = ((PySet*) o)->s;
	Set* partial = ((PySet*) p)->s;

	return make_PySet(recursive_lift(graph, octset, partial), false);
}

static PyObject* vc_lift_recoctlift(PyObject* self, PyObject* args) {
	PyObject* g;
	PyObject* o;
	PyObject* p;

	if (!PyArg_ParseTuple(args, "OOO", &g, &o, &p)) {
		return NULL;
	}

	Graph* graph = ((PyGraph*) g)->g;
	Set* octset = ((PySet*) o)->s;
	Set* partial = ((PySet*) p)->s;

	return make_PySet(recursive_oct_lift(graph, octset, partial), false);
}

static PyObject* vc_lift_recbiplift(PyObject* self, PyObject* args) {
	PyObject* g;
	PyObject* o;
	PyObject* p;

	if (!PyArg_ParseTuple(args, "OOO", &g, &o, &p)) {
		return NULL;
	}

	Graph* graph = ((PyGraph*) g)->g;
	Set* octset = ((PySet*) o)->s;
	Set* partial = ((PySet*) p)->s;

	return make_PySet(recursive_bip_lift(graph, octset, partial), false);
}

static PyMethodDef vc_lift_methods[] = {
	{"naive_lift", vc_lift_naivelift, METH_VARARGS, "computes a complete vertex cover from a partial solution and an octset"},
	{"greedy_lift", vc_lift_greedylift, METH_VARARGS, "computes a complete vertex cover from a partial solution and an octset"},
	{"apx_lift", vc_lift_apxlift, METH_VARARGS, "computes a complete vertex cover from a partial solution and an octset"},
	{"oct_lift", vc_lift_octlift, METH_VARARGS, "computes a complete vertex cover from a partial solution and an octset"},
	{"bip_lift", vc_lift_biplift, METH_VARARGS, "computes a complete vertex cover from a partial solution and an octset"},
	{"recursive_lift", vc_lift_recursivelift, METH_VARARGS, "computes a complete vertex cover from a partial solution and an octset"},
	{"recursive_oct_lift", vc_lift_recoctlift, METH_VARARGS, "computes a complete vertex cover from a partial solution and an octset"},
	{"recursive_bip_lift", vc_lift_recbiplift, METH_VARARGS, "computes a complete vertex cover from a partial solution and an octset"},
	{NULL},
};

static struct PyModuleDef vc_lift_module = {
	PyModuleDef_HEAD_INIT,
	"vc_lift",
	"Python interface for vertex cover algorithms",
	-1,
	vc_lift_methods
};

PyMODINIT_FUNC PyInit_lib_vc_lift() {
	return PyModule_Create(&vc_lift_module);
}
