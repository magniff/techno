#include "Python.h"


line_runner current_line_runner;
line_runner original_line_runner;


int counter = 0;


int
my_function(FILE *fp, PyObject *filename, PyCompilerFlags *flags)
{
    printf("COUNTER: %d\n", counter);
    counter++;
    return PyRun_InteractiveOneObject(fp, filename, flags);
}


PyObject *init_hack(PyObject *self){
    current_line_runner = &my_function;
    return Py_None;
}


PyObject *reset_hack(PyObject *self){
    current_line_runner = original_line_runner;
    return Py_None;
}


#define PUBLIC_METHOD_TYPE (METH_VARARGS|METH_KEYWORDS)
static PyMethodDef techno_functions[] =  {
    {
        "init",
        init_hack,
        METH_VARARGS,
       "Inject library code into CPython`s memory."
    },
    {
        "reset",
        reset_hack,
        METH_VARARGS,
       "Brings back original functionality."
    },
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef techno= {
        PyModuleDef_HEAD_INIT,
        "techno",
        NULL,
        -1,
        techno_functions,
        NULL,
        NULL,
        NULL,
        NULL
};

PyMODINIT_FUNC
PyInit_techno(void)
{
    PyObject *module = PyModule_Create(&techno);
    original_line_runner = current_line_runner;
    return module;
}
