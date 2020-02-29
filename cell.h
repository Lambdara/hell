#ifndef CELL_H
#define CELL_H

typedef struct cell_t {
    int x; // x coord of this cell
    int y; // y coord of this cell
    int neighbour_count; // Size of the following array
    int visited; // Whether this cell has been requested or not
    int answer; // Whether this cell has been marked as part of the answer
    struct cell_t **neighbours; // array of pointers to neighbours
} cell_t;

cell_t *create_cell(int, int);

void destroy_cell(cell_t *);

#endif
