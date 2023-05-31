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

directory = "../get_requests"
filesDirectory = "../get_files"
for file in os.listdir(directory):
    requestFile = open(os.path.join(directory, file), "rb")
    requestFileContent = requestFile.read()
    s.sendall(requestFileContent)
    size = 1
    request = ""
    content = bytearray()
    while size == 1:
        newData = s.recv(1).decode()
        request += newData
        size = len(newData)
        if request.find("\r\n\r\n") != -1:
            print("HEADERS")
            print(request)
            content += bytes(request[request.find("\r\n\r\n")+4:], "utf-8")
            size = 1024
            while size == 1024:
                newData = s.recv(1024)
                content += newData
                size = len(newData)
    extension = file[file.rfind("_")+1:file.find(".")]
    if extension == "dir":
        extension = "dir.txt"
    file1 = open(os.path.join(filesDirectory, "recieved." + extension), "wb")
    file1.write(content)

s.sendall(bytes("GET / HTTP/1.1\r\nConnection: close\r\n\r\n", "utf-8"))
