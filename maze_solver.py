import socket
import struct
from time import sleep

host = 'localhost'
port = 6826

start = (0,0)
goal = (19,19)

# Save paths as [path nodes]
paths = [[start]]

def get_expected_cost(path):
    return len(path) + goal[0] - path[-1][0] + goal[1] - path[-1][1]

def get_neighbours(s,x,y):
    print('Retrieving ({},{})'.format(x,y))
    my_struct = struct.pack('ii',x,y)
    s.send(my_struct)
    neighbour_count = struct.unpack('i', s.recv(struct.calcsize('i')))[0]
    print('Reading {} neighbours'.format(neighbour_count))
    neighbours = []
    for _ in range(neighbour_count):
        retrieved = s.recv(struct.calcsize('ii'))
        [x,y] = struct.unpack('ii', retrieved)[:2]
        neighbours.append((x,y))
    print('Retrieved {}'.format(neighbours))
    return neighbours


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((host,port))

    while paths[-1][-1] != goal:
        path_nodes = paths.pop()
        last_node = path_nodes[-1]

        for neighbour in get_neighbours(s,last_node[0],last_node[1]):
            if neighbour not in path_nodes:
                new_path = list(path_nodes) + [neighbour]
                print(new_path)
                expected_cost = get_expected_cost(new_path)


                paths.append(new_path)

                i = len(paths) - 1
                while i > 0 and get_expected_cost(paths[i-1]) < expected_cost:
                    paths[i-1], paths[i] = paths[i], paths[i-1]
                    i -= 1
    get_neighbours(s,paths[-1][-1][0],paths[-1][-1][1])
    print(paths[-1])
