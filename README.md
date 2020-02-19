# Pymazingl

Pymazingl is an experiment in connecting C, Python (imbedded in C), and OpenGL (through C).

The goal is to have a C program that calls Python code (via the C API for Python) to generate a maze, and then visualizes this maze using OpenGL.

Currently, the maze-generation code is done, and there is a C program which embeds Python in order to use the maze-generation code, which now just prints the loaded-in maze. (And is still very messy an probably has a memory leak.)
