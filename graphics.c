#include "graphics.h"

vector3f create_vector3f(float x, float y, float z) {
    vector3f vector = {x, y, z};
    return vector;
}

void render_scene_cb(GLFWwindow *window, int width, int height, int connections) {
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, (width*height+connections)*6);

    glDisableVertexAttribArray(0);

    glfwSwapBuffers(window);
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

int create_vertex_buffer(int width, int height, cell_t ***cells) {
    int cell_count = width * height;
    int connection_count = 0;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            connection_count += cells[x][y]->neighbour_count;
        }
    }

    int connections = connection_count/2;

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

    // TODO: Communicate connections in a less ugly way
    return connections;
}