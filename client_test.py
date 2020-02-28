import socket
import struct

host = 'localhost'
port = 6826

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((host,port))
    while(True):
        x = int(input('x? '))
        y = int(input('y? '))
        my_struct = struct.pack('ii',x,y)
        s.send(my_struct)
        neighbour_count = struct.unpack('i', s.recv(struct.calcsize('i')))[0]
        for _ in range(neighbour_count):
            [x,y] = struct.unpack('ii', s.recv(struct.calcsize('ii')))[:2]
            print('({},{})'.format(x,y))

