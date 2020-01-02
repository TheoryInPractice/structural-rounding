
#include <Python.h>
#include "setmap.hpp"

// set type //////////////////////////////

typedef struct {
	PyObject_HEAD
	Set* s;
	bool borrowed;
} PySet;

static void Set_dealloc(PySet* self) {
	PyObject_GC_UnTrack(self);
	if (!self->borrowed) {
		delete self->s;
	}
	Py_TYPE(self)->tp_free((PyObject*) self);
}

static int Set_traverse(PyObject* self, visitproc visit, void* arg) {
	return 0;
}

static PyObject* Set_new(PyTypeObject* type, PyObject* args, PyObject* kwrds) {
	PySet* self;
	self = (PySet*) type->tp_alloc(type, 0);
	return (PyObject*) self;
}

static int Set_init(PySet* self) {
	self->s = new Set();
	self->borrowed = false;
	return 0;
}

static PyObject* Set_contains(PySet* self, PyObject* args) {
	int u;
	if (!PyArg_ParseTuple(args, "i", &u)) {
		return NULL;
	}

	if (self->s->contains(u)) {
		Py_RETURN_TRUE;
	}
	else {
		Py_RETURN_FALSE;
	}
}

static PyObject* Set_add(PySet* self, PyObject* args) {
	int u;
	if (!PyArg_ParseTuple(args, "i", &u)) {
		return NULL;
	}

	self->s->insert(u);
	Py_RETURN_NONE;
}

static PyObject* Set_remove(PySet* self, PyObject* args) {
	int u;
	if (!PyArg_ParseTuple(args, "i", &u)) {
		return NULL;
	}

	self->s->erase(u);
	Py_RETURN_NONE;
}

static int Set_contains_in(PySet* self, PyObject* key) {
	int u = PyLong_AsLong(key);
	return self->s->contains(u);
}

static PyObject* Set_size(PySet* self) {
	return PyLong_FromLong(self->s->size());
}

static long Set_size_len(PyObject* self) {
	PySet* temp = (PySet*) self;
	return temp->s->size();
}

static PyMethodDef Set_methods[] = {
	{"contains", (PyCFunction) Set_contains, METH_VARARGS, "checks if element in set"},
	{"__contains__", (PyCFunction) Set_contains, METH_VARARGS, "checks if element in set"},
	{"size", (PyCFunction) Set_size, METH_NOARGS, "gets number of elements in the set"},
	{"__sizeof__", (PyCFunction) Set_size, METH_NOARGS, "gets number of elements in the set"},
	{"add", (PyCFunction) Set_add, METH_VARARGS, "adds an element to the set"},
	{"remove", (PyCFunction) Set_remove, METH_VARARGS, "removes an element from the set"},
	{NULL},
};

static PySequenceMethods Set_sequence_methods = {
	.sq_length = Set_size_len,
	.sq_contains = (objobjproc) Set_contains_in,
};

// set iter type ///////////////////////////

typedef struct {
	PyObject_HEAD
	PySet* s;
	Set::Iterator current;
	int len;
} PySetIter;

static void SetIter_dealloc(PySetIter* self) {
	PyObject_GC_UnTrack(self);
	Py_XDECREF(self->s);
	PyObject_GC_Del(self);
}

static int SetIter_traverse(PyObject* self, visitproc visit, void* arg) {
	PySetIter* temp = (PySetIter*) self;
	Py_VISIT(temp->s);
	return 0;
}

static PyObject* SetIter_len(PySetIter* self) {
	int len = 0;
	if (self->s != NULL) {
		len = self->len;
	}

	return PyLong_FromLong(len);
}

static PyObject* SetIter_iternext(PySetIter* self) {
	if (self->current == self->s->s->end()) {
		PySet* temp = self->s;
		self->s = NULL;
		Py_DECREF(temp);
		PyErr_SetNone(PyExc_StopIteration);
		return NULL;
	}

	PyObject* ret = PyLong_FromLong(*(self->current));

	++(self->current);
	--(self->len);

	return ret;
}

static PyMethodDef SetIter_methods[] = {
	{"__length_hint__", (PyCFunction) SetIter_len, METH_NOARGS, "gets remaining elements in sequence"},
	{},
};

PyTypeObject SetIter_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "setmap.SetIterator",
	.tp_basicsize = sizeof(PySetIter),
	.tp_itemsize = 0,
	.tp_dealloc = (destructor) SetIter_dealloc,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,
	.tp_doc = "Set iterator object",
	.tp_traverse = SetIter_traverse,
	.tp_iter = PyObject_SelfIter,
	.tp_iternext = (iternextfunc) SetIter_iternext,
	.tp_methods = SetIter_methods,
};

static PyObject* Set_iter(PySet* self) {
	PySetIter* iter = PyObject_GC_New(PySetIter, &SetIter_type);
	if (iter == NULL) {
		return NULL;
	}

	Py_INCREF(self);
	iter->s = self;
	iter->current = Set::Iterator(self->s);
	iter->len = self->s->size();
	PyObject_GC_Track(iter);
	return (PyObject*) iter;
}

static PyTypeObject Set_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "setmap.Set",
	.tp_basicsize = sizeof(PySet),
	.tp_itemsize = 0,
	.tp_dealloc = (destructor) Set_dealloc,
	.tp_as_sequence = &Set_sequence_methods,
	.tp_hash = PyObject_HashNotImplemented,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
	.tp_doc = "Set object",
	.tp_traverse = Set_traverse,
	.tp_iter = (getiterfunc) Set_iter,
	.tp_methods = Set_methods,
	.tp_init = (initproc) Set_init,
	.tp_alloc = PyType_GenericAlloc,
	.tp_new = Set_new,
	.tp_free = PyObject_GC_Del,
};

static PyModuleDef setmap_module = {
	PyModuleDef_HEAD_INIT,
	.m_name = "setmap",
	.m_doc = "module for sets",
	.m_size = -1,
};

PyMODINIT_FUNC PyInit_setmap() {
	PyObject* m;
	if (PyType_Ready(&Set_type) < 0 || PyType_Ready(&SetIter_type) < 0) {
		return NULL;
	}

	m = PyModule_Create(&setmap_module);
	if (m == NULL) {
		return NULL;
	}

	Py_INCREF(&Set_type);
	if (PyModule_AddObject(m, "Set", (PyObject*) &Set_type) < 0) {
		Py_DECREF(&Set_type);
		Py_DECREF(m);
		return NULL;
	}

	Py_INCREF(&SetIter_type);
	if (PyModule_AddObject(m, "SetIterator", (PyObject*) &SetIter_type) < 0) {
		Py_DECREF(&SetIter_type);
		Py_DECREF(m);
		return NULL;
	}

    return m;
}
