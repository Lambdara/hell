# Hell

Hell is an experiment in connecting C, Python (imbedded in C), and OpenGL (through C).

The original goal was to have a C program that calls Python code (via the C API for Python) to generate a maze, and then visualizes this maze using OpenGL.

This now works, though the code isn't really high quality.

Current aims are:
- Higher code quality
- Further overcomplicating an easy task by having some program solve the maze by communicating with the program over TCP and visualizing this process in real time. (The ability to get information from the server on which cells have which neighbours is now implemented, see `client_test.py` for example on usage)

Dependencies/Requirements:
The `Makefile` assumes you have Python 3.8, all libraries for building C with embedded Python 3.8 (see output of `python3.8-config --ldflags --libs --embed --cflags` on your system) as well as a number of graphics-related libraries, most significantly OpenGL, GLEW and GLFW.
