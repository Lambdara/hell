#define PY_SSIZE_T_CLEAN
#include "cell.h"
#include "networking.h"
#include "graphics.h"
#include "maze_import.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

cell_t ***cells;
GLFWwindow *window;
int width = 96, height = 54;

struct msg {
    int x;
    int y;
};

void send_neighbours(int conn_fd, int x, int y) {
    struct msg response;

    int neighbour_count = cells[x][y]->neighbour_count;

    send(conn_fd, &neighbour_count, sizeof(neighbour_count), 0);
    for (int i = 0; i < neighbour_count; i++) {
        response.x = cells[x][y]->neighbours[i]->x;
        response.y = cells[x][y]->neighbours[i]->y;
        send(conn_fd, &response, sizeof(response), 0);
    }
}

void *do_networking(void *vars) {
    // Connect to client
    int sockfd;
    int conn_fd = connect_to_client(&sockfd);

    struct msg message;
    int nbytes;

    message.x = width;
    message.y = height;
    send(conn_fd, &message, sizeof(message), 0);
    while((nbytes = read(conn_fd, &message, sizeof(message))) > 0) {
        if (nbytes > 0) {
            cells[message.x][message.y]->visited = 1;

            // Asking about the goal indicates wanting to give the answer
            if (message.x != width - 1 || message.y != height -1)
                send_neighbours(conn_fd, message.x, message.y);
            else {
                int incoming_length;
                read(conn_fd, &incoming_length, sizeof(incoming_length));
                for (int i = 0; i < incoming_length; i++) {
                    read(conn_fd, &message, sizeof(message));
                    cells[message.x][message.y]->answer = 1;
                }
            }
        }
    }

    // Print the error if we encounter one
    if (nbytes < 0) {
        printf("%i bytes\n", nbytes);
        printf("%s\n",strerror(errno));
    }

    // Close when we're done
    close(sockfd);
    printf("Socket closed\n");

    return NULL;
}

int main(int argc, char *argv[]) {
    // Initialize the maze
    cells = malloc(width * sizeof(cell_t**));

    for (int x = 0; x < width; x++){
        cells[x] = malloc(height * sizeof(cell_t**));
        for(int y = 0; y < height; y++){
            cells[x][y] = create_cell(x,y);
        }
    }

    load_maze_from_python(width, height, cells);

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, do_networking, NULL);
    // Initialize GLFW Window
    if (!glfwInit()) {
        printf("GLFW failed to init\n");
        return -1;
    }
    window = glfwCreateWindow(1920, 1080, "Hell", NULL, NULL);
    if (!window) {
        printf("Window failed to create\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    add_shaders();

    // Create the image
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glGenBuffers(1, &cbo);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vector3f)*6*(width*height*2-1), NULL, GL_DYNAMIC_DRAW);
    glGenBuffers(1, &vbo);

    create_vertex_buffer(width, height, cells);
    create_color_buffer(width, height, cells);

    // Render the screen until we close
    while(!glfwWindowShouldClose(window)) {
        create_color_buffer(width, height, cells);
        render_scene_cb(window, width, height);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &cbo);

    return 0;
}
