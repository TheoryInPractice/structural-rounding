
#include <Python.h>

#include "lp_kernel.hpp"
#include "pygraph.hpp"
#include "pyset.hpp"
#include "graph.hpp"

static PyObject* vc_kernel_lpkernel(PyObject* self, PyObject* args) {
    PyObject* g;
    if (!PyArg_ParseTuple(args, "O", &g)) {
        return NULL;
    }

    Graph* graph = ((PyGraph*) g)->g;
    return make_PySet(lp_kernel(graph), false);
}

static PyMethodDef lp_kernel_methods[] = {
    {"lp_kernel", vc_kernel_lpkernel, METH_VARARGS, "computes a kernel for a bipartite graph using lp flow kernelization"},
    {NULL},
};

static struct PyModuleDef lp_kernel_module = {
    PyModuleDef_HEAD_INIT,
    "lp_kernel",
    "Python interface for lp kernel",
    -1,
    lp_kernel_methods
};

PyMODINIT_FUNC PyInit_lib_lp_kernel() {
    return PyModule_Create(&lp_kernel_module);
}
