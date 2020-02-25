#define PY_SSIZE_T_CLEAN
#include "cell.h"
#include <Python.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

cell_t ***cells;
int width = 20, height = 20, connections = 0;

int load_maze_from_python(int width, int height, cell_t *** cells) {
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
        fprintf(stderr, "Failed to finalize\n");
        return 1;
    }

    return 0;
}


GLuint vbo;

typedef struct {
    float x, y, z;
} vector3f;

vector3f create_vector3f(float x, float y, float z) {
    vector3f vector = {x, y, z};
    return vector;
}

void render_scene_cb()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, (width*height+connections)*6);

    glDisableVertexAttribArray(0);

    glutSwapBuffers();
}

void initialize_glut_callbacks()
{
    glutDisplayFunc(render_scene_cb);
}

void add_cell_vertices(
     int x,
     int y,
     float width_interval,
     float height_interval,
     vector3f *vertices,
     int *i
    ) {
    vertices[(*i)++] = create_vector3f(-1.0f + width_interval * 2 * x,
                                       -1.0f + height_interval * 2 * y,
                                       0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + ((x * 2) + 1) * width_interval,
                                       -1.0f + height_interval * 2 * y,
                                       0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + ((x * 2) + 1) * width_interval,
                                       -1.0f + ((y * 2) + 1) * height_interval,
                                       0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + ((x * 2) + 1) * width_interval,
                                       -1.0f + ((y * 2) + 1) * height_interval,
                                       0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + width_interval * 2 * x,
                                       -1.0f + ((y * 2) + 1) * height_interval,
                                       0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + width_interval * 2 * x,
                                       -1.0f + height_interval * 2 * y,
                                       0.0f);
}

// Adds the vertices for a vertical connection on top of the cell at (x,y)
void add_vertical_connection_vertices(
    int x,
    int y,
    float width_interval,
    float height_interval,
    vector3f *vertices,
    int *i
    ) {
    float width_interval_third = width_interval / 3;
    vertices[(*i)++] = create_vector3f(-1.0f + (x * 2) * width_interval + width_interval_third,
                                       -1.0f + ((y * 2) + 1) * height_interval,
                                       0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + ((x * 2) + 1) * width_interval - width_interval_third,
                                       -1.0f + ((y * 2) + 1) * height_interval,
                                       0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + ((x * 2) + 1) * width_interval - width_interval_third,
                                       -1.0f + ((y + 1) * 2) * height_interval,
                                       0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + ((x * 2) + 1) * width_interval - width_interval_third,
                                       -1.0f + ((y + 1) * 2) * height_interval,
                                       0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + (x * 2) * width_interval + width_interval_third,
                                       -1.0f + ((y + 1) * 2) * height_interval,
                                       0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + (x * 2) * width_interval + width_interval_third,
                                       -1.0f + ((y * 2) + 1) * height_interval,
                                       0.0f);
}

// Adds the vertices for a horizontal connection to the right of the cell at (x,y)
void add_horizontal_connection_vertices(
    int x,
    int y,
    float width_interval,
    float height_interval,
    vector3f *vertices,
    int *i
    ) {
    float height_interval_third = height_interval / 3;
    vertices[(*i)++] = create_vector3f(-1.0f + ((x * 2) + 1) * width_interval,
                                    -1.0f + height_interval * 2 * y + height_interval_third,
                                    0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + ((x + 1) * 2) * width_interval,
                                    -1.0f + height_interval * 2 * y + height_interval_third,
                                    0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + ((x + 1) * 2) * width_interval,
                                    -1.0f + ((y * 2) + 1) * height_interval - height_interval_third,
                                    0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + ((x + 1) * 2) * width_interval,
                                    -1.0f + ((y * 2) + 1) * height_interval - height_interval_third,
                                    0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + ((x * 2) + 1) * width_interval,
                                    -1.0f + ((y * 2) + 1) * height_interval - height_interval_third,
                                    0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + ((x * 2) + 1) * width_interval,
                                    -1.0f + height_interval * 2 * y + height_interval_third,
                                    0.0f);
}

void create_vertex_buffer()
{
    int cell_count = width * height;
    int connection_count = 0;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            connection_count += cells[x][y]->neighbour_count;
        }
    }

    connections = connection_count/2;

    vector3f vertices[(cell_count+connection_count)*6];

    float width_interval = 2.0 / (width * 2.0 - 1.0);
    float height_interval = 2.0 / (height * 2.0 - 1.0);

    int i = 0;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            add_cell_vertices(x, y, width_interval, height_interval, vertices, &i);
            for (int n = 0; n < cells[x][y]->neighbour_count; n++) {
                cell_t* neighbour = cells[x][y]->neighbours[n];
                if (neighbour->x == x + 1) {
                    add_horizontal_connection_vertices(x, y, width_interval, height_interval, vertices, &i);
                } else if (neighbour->y == y + 1) {
                    add_vertical_connection_vertices(x, y, width_interval, height_interval, vertices, &i);
                }
            }
        }
    }

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

int main(int argc, char *argv[]) {
    cells = malloc(width * sizeof(cell_t**));

    for (int x = 0; x < width; x++){
        cells[x] = malloc(height * sizeof(cell_t**));
        for(int y = 0; y < height; y++){
            cells[x][y] = create_cell(x,y);
        }
    }

    load_maze_from_python(width, height, cells);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hell");

    initialize_glut_callbacks();

    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    create_vertex_buffer();

    glutMainLoop();

    return 0;
}
