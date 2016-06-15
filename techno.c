#include "Python.h"
#include <stdint.h>


line_runner current_line_runner;
line_runner original_line_runner;


frame_evaluator current_frame_evaluator;
frame_evaluator original_frame_evaluator;


static int64_t COUNTER_PREV = 0;
static int64_t COUNTER_CURR = 0;


int
my_repl_handler(FILE *fp, PyObject *filename, PyCompilerFlags *flags)
{
    int result = PyRun_InteractiveOneObject(fp, filename, flags);
    printf("Frames evaluated: %d\n", COUNTER_CURR - COUNTER_PREV);
    COUNTER_PREV = COUNTER_CURR = 0;
    return result;
}

PyObject *
my_evaluator(struct _frame *f, int exc)
{
    COUNTER_CURR++;
    return PyEval_EvalFrameEx(f, exc);
}


PyObject *init_hack(PyObject *self){
    current_line_runner = &my_repl_handler;
    current_frame_evaluator = &my_evaluator;
    return Py_None;
}


PyObject *reset_hack(PyObject *self){
    current_line_runner = original_line_runner;
    current_frame_evaluator = original_frame_evaluator;
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
    original_frame_evaluator= current_frame_evaluator;
    return module;
}
