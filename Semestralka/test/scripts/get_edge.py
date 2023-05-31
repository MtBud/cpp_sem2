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

directory = "../get_edge"
filesDirectory = "../get_files"
for file in os.listdir(directory):
    requestFile = open(os.path.join(directory, file), "rb")
    requestFileContent = requestFile.read()
    s.sendall(requestFileContent)
    size = 1024
    request = ""
    content = bytearray()
    while size == 1024:
        newData = s.recv(1024)
        content += newData
        size = len(newData)
        print("content:")
        print(content.decode())


    # extension = file[file.rfind("_")+1:file.find(".")]
    # file1 = open(os.path.join(filesDirectory, "recieved." + extension), "wb")
    # file1.write(content)

s.sendall(bytes("GET / HTTP/1.1\r\nConnection: close\r\n\r\n", "utf-8"))
