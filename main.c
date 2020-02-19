#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    setenv("PYTHONPATH",".",1);

    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;

    char *module_name= "generate_maze";
    char *function_name = "generate_maze";
    long width = 10;
    long height = 10;

    // Initialize Python and load module
    Py_Initialize();
    pName = PyUnicode_DecodeFSDefault(module_name);
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        // Load function
        pFunc = PyObject_GetAttrString(pModule, function_name);

        if (pFunc && PyCallable_Check(pFunc)) {
            // Create a tuple and put in the function arguments
            pArgs = PyTuple_New(2);
            pValue = PyLong_FromLong(width);
            PyTuple_SetItem(pArgs, 0, pValue);
            pValue = PyLong_FromLong(height);
            PyTuple_SetItem(pArgs, 1, pValue);

            // Call the function and get the result
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                printf("Result of call: %ld\n", PyLong_AsLong(pValue));
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return 1;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
        return 1;
    }
    if (Py_FinalizeEx() < 0) {
        return 120;
    }
    return 0;
}
