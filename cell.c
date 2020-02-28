#include "cell.h"
#include "graphics.h"
#include <stdlib.h>

cell_t *create_cell(int x, int y){
    cell_t *cell = malloc(sizeof(cell_t));
    cell->x = x;
    cell->y = y;
    cell->visited = 0;

    return cell;
}

void destroy_cell(cell_t *cell){
    free(cell->neighbours);
    free(cell);
}
