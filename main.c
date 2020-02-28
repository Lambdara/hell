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
int width = 20, height = 20, connections = 0;

struct msg {
    int x;
    int y;
};

void *do_networking(void *vars) {
    // Connect to client
    int sockfd;
    int conn_fd = connect_to_client(&sockfd);

    struct msg message;
    int nbytes;

    // Spy stub code for communication with client
    while((nbytes = read(conn_fd, &message, sizeof(message))) > 0) {
        if (nbytes > 0) {
            printf("(%i,%i)\n", message.x, message.y);
            cells[message.x][message.y]->visited = 1;
            for (int i = 0; i < cells[message.x][message.y]->neighbour_count; i++) {
                struct msg response = {
                                       cells[message.x][message.y]->neighbours[i]->x,
                                       cells[message.x][message.y]->neighbours[i]->y
                };
                send(conn_fd, &response, sizeof(response), 0);
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
    window = glfwCreateWindow(1024, 768, "Hell", NULL, NULL);
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
    connections = create_vertex_buffer(width, height, cells);

    // Render the screen until we close
    while(!glfwWindowShouldClose(window)) {
        create_vertex_buffer(width,height,cells);
        render_scene_cb(window, width, height, connections);
        glfwPollEvents();
    }

    return 0;
}
