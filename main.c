#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdlib.h>

struct pair_t {
   int x;
   int y;
};

int main(int argc, char *argv[]) {
    setenv("PYTHONPATH",".",1);

    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;

    char *module_name= "generate_maze";
    char *function_name = "generate_maze";
    long width = 3;
    long height = 3;

    // Cells of maze, stored as adjacency matrix
    struct pair_t *cells[width][height];
    int cell_sizes[width][height];

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
                // Read it, just read it.
                for (int x = 0; x < width; x++) {
                    for (int y = 0; y < height; y++) {
                        PyObject *pyx = PyLong_FromLong(x);
                        PyObject *pyy = PyLong_FromLong(y);
                        PyObject *list = PyList_GetItem(PyList_GetItem(pValue,PyLong_AsSsize_t(pyx)),PyLong_AsSsize_t(pyy));
                        long list_size = PyLong_AsLong(PyLong_FromSsize_t(PyList_Size(list)));

                        cells[x][y] = malloc(list_size * sizeof(struct pair_t));
                        cell_sizes[x][y] = list_size;

                        for (int i = 0; i < list_size; i++) {
                            PyObject *pyi = PyLong_FromLong(i);
                            PyObject *pair = PyList_GetItem(list, PyLong_AsSsize_t(pyi));
                            PyObject *pairx = PyTuple_GetItem(pair, 0);
                            PyObject *pairy = PyTuple_GetItem(pair, 1);
                            cells[x][y][i].x = PyLong_AsLong(pairx);
                            cells[x][y][i].y = PyLong_AsLong(pairy);
                        }
                    }
                }
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

    for (int x = 0; x < width; x++){
        for (int y = 0; y < height; y++){
            printf("Adjacents of (%d,%d):\n",x,y);
            for (int i = 0; i < cell_sizes[x][y] ; i++){
                printf("(%d,%d)\n",cells[x][y][i].x,cells[x][y][i].y);
            }
        }
    }

    return 0;
}
