#include "graphics.h"

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
    // Read vertex shader from file
    char *vertex_shader_source;
    FILE *vertex_shader_file = fopen(VERTEX_SHADER_PATH, "rb");
    if (vertex_shader_file) {
        fseek(vertex_shader_file, 0, SEEK_END);
        long length = ftell(vertex_shader_file);
        fseek (vertex_shader_file, 0, SEEK_SET);
        vertex_shader_source = malloc (length);
        fread(vertex_shader_source, 1, length, vertex_shader_file);
        fclose(vertex_shader_file);
    } else {
        fprintf(stderr, "Could not open vertex shader file\n");
        exit(1);
    }
    const char *vertex_shader_content = vertex_shader_source;

    // Compile vertex shader
    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_content, NULL);
    glCompileShader(vertex_shader);
    int  success;
    char info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        fprintf(stderr,"Shader compilation error: %s\n", info_log);
    }

    // Load fragment shader from file
    char *fragment_shader_source;
    FILE *fragment_shader_file = fopen(FRAGMENT_SHADER_PATH, "rb");
    if (fragment_shader_file) {
        fseek (fragment_shader_file, 0, SEEK_END);
        long length = ftell(fragment_shader_file);
        fseek (fragment_shader_file, 0, SEEK_SET);
        fragment_shader_source = malloc(length);
        fread(fragment_shader_source, 1, length, vertex_shader_file);
        fclose(fragment_shader_file);
    } else {
        fprintf(stderr, "Could not open fragment shader file\n");
        exit(10);
    }
    const char *fragment_shader_content = fragment_shader_source;
    
    // Compile fragment_shader
    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_content, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        fprintf(stderr,"Shader compilation error: %s\n", info_log);
    }

    // Link shader program
    unsigned int shader_program;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramInfoLog(shader_program, 512, NULL, info_log);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        fprintf(stderr,"Shader program error: %s\n", info_log);
    }

    glUseProgram(shader_program);
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
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors), colors);
}
