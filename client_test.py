import socket

host = 'localhost'
port = 6824

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((host,port))
    s.send(b'Big oof')
    input("press enter to close socket")
    s.close()
