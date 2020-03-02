#include "graphics.h"

const char *vertexShaderSource = "#version 330\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 vertexColor;\n"
    "out vec3 fragmentColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   fragmentColor = vertexColor;\n"
    "}\0";

const char *fragmentShaderSource = "#version 330\n"
    "in vec3 fragmentColor;\n"
    "out vec3 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = fragmentColor;\n"
    "}\n";

vector3f create_vector3f(float x, float y, float z) {
    vector3f vector = {x, y, z};
    return vector;
}

void render_scene_cb(GLFWwindow *window, int width, int height) {
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, (width*height*2-1)*6);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

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
    vertices[(*i)++] = create_vector3f(-1.0f + (x * 2) * width_interval,
                                       -1.0f + ((y * 2) + 1) * height_interval,
                                       0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + ((x * 2) + 1) * width_interval,
                                       -1.0f + ((y * 2) + 1) * height_interval,
                                       0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + ((x * 2) + 1) * width_interval,
                                       -1.0f + ((y + 1) * 2) * height_interval,
                                       0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + ((x * 2) + 1) * width_interval,
                                       -1.0f + ((y + 1) * 2) * height_interval,
                                       0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + (x * 2) * width_interval,
                                       -1.0f + ((y + 1) * 2) * height_interval,
                                       0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + (x * 2) * width_interval,
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
    vertices[(*i)++] = create_vector3f(-1.0f + ((x * 2) + 1) * width_interval,
                                       -1.0f + height_interval * 2 * y,
                                       0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + ((x + 1) * 2) * width_interval,
                                       -1.0f + height_interval * 2 * y,
                                       0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + ((x + 1) * 2) * width_interval,
                                       -1.0f + ((y * 2) + 1) * height_interval,
                                       0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + ((x + 1) * 2) * width_interval,
                                       -1.0f + ((y * 2) + 1) * height_interval,
                                       0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + ((x * 2) + 1) * width_interval,
                                       -1.0f + ((y * 2) + 1) * height_interval,
                                       0.0f);
    vertices[(*i)++] = create_vector3f(-1.0f + ((x * 2) + 1) * width_interval,
                                       -1.0f + height_interval * 2 * y,
                                       0.0f);
}

void add_shaders() {
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int  success;
    char info_log[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, info_log);
        fprintf(stderr,"Shader compilation error: %s\n", info_log);
    }
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, info_log);
        fprintf(stderr,"Shader compilation error: %s\n", info_log);
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramInfoLog(shaderProgram, 512, NULL, info_log);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, info_log);
        fprintf(stderr,"Shader program error: %s\n", info_log);
    }

    glUseProgram(shaderProgram);

    fprintf(stderr, "Shaders added\n");
}

void create_vertex_buffer(int width, int height, cell_t ***cells) {
    vector3f vertices[(width*height*2-1)*6];

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
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void create_color_buffer(int width, int height, cell_t ***cells) {
    vector3f colors[(width*height*2-1)*6];

    int i = 0;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            for (int j = i; j < i + 6; j++)
                if (cells[x][y]->answer)
                    colors[j] = create_vector3f(0.0f,0.392f,0.0f);
                else if (cells[x][y]->visited)
                    colors[j] = create_vector3f(1.0f,0.3f,0.3f);
                else
                    colors[j] = create_vector3f(0.8f,0.8f,1.0f);
            i += 6;
            for (int n = 0; n < cells[x][y]->neighbour_count; n++) {
                cell_t* neighbour = cells[x][y]->neighbours[n];
                if (neighbour->x == x + 1 || neighbour->y == y + 1){
                    for (int j = i; j < i + 6; j++){
                        if (cells[x][y]->answer && neighbour->answer)
                            colors[j] = create_vector3f(0.0f,0.392f,0.0f);
                        else if ((cells[x][y]->visited || cells[x][y]->answer) && (neighbour->visited || neighbour->answer))
                            colors[j] = create_vector3f(1.0f,0.3f,0.3f);
                        else
                            colors[j] = create_vector3f(0.8f,0.8f,1.0f);
                    }
                    i += 6;
                }
            }
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);
}
