#ifndef MAZE_IMPORT_H
#define MAZE_IMPORT_H

#include <Python.h>
#include "cell.h"

int load_maze_from_python(int width, int height, cell_t ***cells);

#endif
