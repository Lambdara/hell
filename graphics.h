#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "cell.h"

GLuint vbo, cbo;

typedef struct {
    float x, y, z;
} vector3f;

vector3f create_vector3f(float, float, float);

void render_scene_cb(GLFWwindow *, int, int, int);

void add_cell_vertices(int, int, float, float, vector3f *, int *);

void add_vertical_connection_vertices(int, int, float, float, vector3f *, int *);

void add_horizontal_connection_vertices(int, int, float, float, vector3f *, int *);

int create_vertex_buffer(int, int, cell_t ***);

void add_shaders();

#endif
