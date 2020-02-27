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


    // Child will handle networking, while parent handles graphics
    pid_t child_pid = fork();
    if (child_pid == 0) {
        // Connect to client
        int sockfd;
        int conn_fd = connect_to_client(&sockfd);

        char buffer[CONNECTION_BUFFER_SIZE];
        int nbytes;

        // Spy stub code for communication with client
        while((nbytes = read(conn_fd, buffer, CONNECTION_BUFFER_SIZE)) > 0) {
            if (nbytes > 0)
                printf("%i bytes: %s\n", nbytes, buffer);
        }

        // Print the error if we encounter one
        if (nbytes < 0) {
            printf("%i bytes\n", nbytes);
            printf("%s\n",strerror(errno));
        }

        // Close when we're done
        close(sockfd);
        printf("Socket closed\n");
    } else {
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

        // Create the image
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        connections = create_vertex_buffer(width, height, cells);

        // Render the screen until we close
        while(!glfwWindowShouldClose(window)) {
            render_scene_cb(window, width, height, connections);
            glfwPollEvents();
        }

        // Make sure our child is dead (inter-process infanticide is no joke!)
        kill(child_pid, SIGTERM);
    }

    return 0;
}
