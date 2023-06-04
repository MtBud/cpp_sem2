import socket
import os

try:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print("Socket successfully created")
except socket.error as err:
    print("socket creation failed with error %s" % err)
    exit()

port = 8080
address = '127.0.0.1'

s.connect((address, port))
print("successful connection")

directory = "../exec_scripts"
for file in os.listdir(directory):
    if os.path.isdir(file):
        continue
    requestFile = open(os.path.join(directory, file), "rb")
    requestFileContent = requestFile.read()
    s.sendall(requestFileContent)
    size = 1024
    request = ""
    content = bytearray()
    while size == 1024:
        newData = s.recv(1024).decode()
        request += newData
        size = len(newData)

    print(request)
