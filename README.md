# Hell

Hell is an experiment in connecting C, Python (imbedded in C), and OpenGL (through C).

The original goal was to have a C program that calls Python code (via the C API for Python) to generate a maze, and then visualizes this maze using OpenGL.

There is an additional Python program `maze_solver.py`, which makes a connection over a socket to the C program, and asks it for the neighbours of cells to solve the maze.

Current aims are:
- Higher code quality

Dependencies/Requirements:
The `Makefile` assumes you have Python 3.8, all libraries for building C with embedded Python 3.8 (see output of `python3.8-config --ldflags --libs --embed --cflags` on your system) as well as a number of graphics-related libraries, most significantly OpenGL, GLEW and GLFW.
