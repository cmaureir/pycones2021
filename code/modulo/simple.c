#include <Python.h>

static PyObject* hola(PyObject* self, PyObject* args){
    char *msg = "¡Hola PyConES 2021!";
    return Py_BuildValue("s", msg);
}

static PyMethodDef funciones[] = {
    {"hola", (PyCFunction)hola, METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef modulo = {
    PyModuleDef_HEAD_INIT,
    "pycones",
    NULL,
    -1,
    funciones
};

PyMODINIT_FUNC PyInit_pycones(void){
    return PyModule_Create(&modulo);
}
