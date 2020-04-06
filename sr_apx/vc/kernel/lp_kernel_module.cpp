
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

    Set** kernel = lp_kernel(graph);
    PyObject* in = make_PySet(kernel[0], false);
    PyObject* out = make_PySet(kernel[1], false);
    delete[] kernel;
    return Py_BuildValue("OO", in, out);
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
