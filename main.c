#define PY_SSIZE_T_CLEAN
#include "cell.h"
#include <Python.h>
#include <stdlib.h>

int load_maze_from_python( int width, int height,cell_t * cells[][height]) {
    setenv("PYTHONPATH",".",1);

    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;

    char *module_name= "generate_maze";
    char *function_name = "generate_maze";

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

                        cells[x][y]->neighbour_count = (int) list_size;
                        cells[x][y]->neighbours = malloc(list_size * sizeof(cell_t *));

                        for (int i = 0; i < list_size; i++) {
                            PyObject *pyi = PyLong_FromLong(i);
                            PyObject *pair = PyList_GetItem(list, PyLong_AsSsize_t(pyi));
                            int pairx = PyLong_AsLong(PyTuple_GetItem(pair, 0));
                            int pairy = PyLong_AsLong(PyTuple_GetItem(pair, 1));
                            cells[x][y]->neighbours[i] = cells[pairx][pairy];
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
            fprintf(stderr, "Cannot find function\n");
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load module\n");
        return 1;
    }
    if (Py_FinalizeEx() < 0) {
        return 120;
    }

    for (int x = 0; x < width; x++){
        for (int y = 0; y < height; y++){
            printf("Adjacents of (%d,%d):\n",x,y);
            for (int i = 0; i < cells[x][y]->neighbour_count ; i++){
                printf("(%d,%d)\n",cells[x][y]->neighbours[i]->x,cells[x][y]->neighbours[i]->y);
            }
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int width = 3, height = 3;

    cell_t *cells[width][height];

    for (int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            cells[x][y] = create_cell(x,y);
        }
    }

    load_maze_from_python(width, height,cells);

    return 0;
}
