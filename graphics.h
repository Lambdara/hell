#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "cell.h"

#define VERTEX_SHADER_PATH "vertexshader.vs"
#define FRAGMENT_SHADER_PATH "fragmentshader.fs"

GLuint vbo, cbo;

typedef struct {
    float x, y, z;
} vector3f;

int vbuffer_size;
vector3f *colors;
vector3f *vertices;


vector3f create_vector3f(float, float, float);

void render_scene_cb(GLFWwindow *, int, int);

void add_cell_vertices(int, int, float, float, vector3f *, int *);

void add_vertical_connection_vertices(int, int, float, float, vector3f *, int *);

void add_horizontal_connection_vertices(int, int, float, float, vector3f *, int *);

void create_vertex_buffer(int, int, cell_t ***);

void create_color_buffer(int, int, cell_t ***);

void add_shaders();

void initialize_graphics_buffers(int, int);

#endif
