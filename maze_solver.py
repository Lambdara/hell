import socket
import struct
from time import sleep
import sys

host = 'localhost'
port = 6826

start = (0,0)
goal = (0,0)

# Save paths as [path nodes]
paths = [[start]]

if len(sys.argv) > 1:
    sleep_time = float(sys.argv[1])
else:
    sleep_time = 0.0

def get_expected_cost(path):
    return abs(goal[0] - path[-1][0]) + abs(goal[1] - path[-1][1])

def get_neighbours(s,x,y):
    sleep(sleep_time)
    my_struct = struct.pack('ii',x,y)
    s.send(my_struct)
    neighbour_count = struct.unpack('i', s.recv(struct.calcsize('i')))[0]
    neighbours = []
    for _ in range(neighbour_count):
        retrieved = s.recv(struct.calcsize('ii'))
        [x,y] = struct.unpack('ii', retrieved)[:2]
        neighbours.append((x,y))
    return neighbours


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((host,port))

    retrieved = s.recv(struct.calcsize('ii'))
    [width, height] = struct.unpack('ii', retrieved)[:2]
    goal = (width-1, height-1)

    while paths[-1][-1] != goal:
        path_nodes = paths.pop()
        last_node = path_nodes[-1]

        for neighbour in get_neighbours(s,last_node[0],last_node[1]):
            if neighbour not in path_nodes:
                new_path = list(path_nodes) + [neighbour]
                expected_cost = get_expected_cost(new_path)


                paths.append(new_path)

                i = len(paths) - 1
                while i > 0 and get_expected_cost(paths[i-1]) < expected_cost:
                    paths[i-1], paths[i] = paths[i], paths[i-1]
                    i -= 1

    answer = paths[-1]
    s.send(struct.pack('ii', answer[-1][0], answer[-1][1]))

    outgoing_size_struct = struct.pack('i',len(answer))
    s.send(outgoing_size_struct)

    for (x,y) in answer:
        cell = struct.pack('ii',x,y)
        s.send(cell)
