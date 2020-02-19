from random import shuffle

class Cell:
    def __init__(self,x,y):
        self.x = x
        self.y = y
        self.connected_neighbours = []
        self.neighbours = []

    def __string__(self):
        return 'Cell ({},{})'.format(self.x,self.y)

    def __repr__(self):
        return self.__string__()


def generate_maze(width,height,start=(0,0)):
    # Initialize cells
    cells = [[Cell(x,y) for y in range(height)] for x in range(width)]

    # In the trivial case we are done (and searching wouldn't work so return now)
    if width == height == 1:
        return cells

    # Assign cells their neighbours
    for x in range(height):
        for y in range(width):
            neighbours = []
            if x > 0:
                neighbours.append(cells[x-1][y])
            if x < width - 1:
                neighbours.append(cells[x+1][y])
            if y > 0:
                neighbours.append(cells[x][y-1])
            if y < height - 1:
                neighbours.append(cells[x][y+1])
            cells[x][y].neighbours = neighbours

    # Search from start to every other cell in random order to generate a maze
    start_cell = cells[start[0]][start[1]]

    stack = [start_cell]
    visited = [start_cell]

    while stack:
        cell = stack.pop()

        shuffle(cell.neighbours)
        for neighbour in cell.neighbours:
            if neighbour not in visited:
                stack.append(neighbour)
                visited.append(neighbour)
                cell.connected_neighbours.append(neighbour)
                neighbour.connected_neighbours.append(cell)

    return cells
