import socket
import os
import re

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
headerTerminator = b'\x0d\x0a\x0d\x0a'
for file in os.listdir(directory):
    requestFile = open(os.path.join(directory, file), "rb")
    requestFileContent = requestFile.read()
    s.sendall(requestFileContent)
    size = 1
    request = bytearray()
    content = bytearray()
    while size == 1:
        newData = s.recv(1)
        request += newData
        size = len(newData)
        if headerTerminator in request:
            print("HEADERS")
            print(request.decode())
            contentLength = int(re.search(r'\d+', request.decode()[request.decode().index("Content-Length: ") + len("Content-Length: "):]).group())
            content += request[request.index(headerTerminator) + 4:]
            while contentLength != len(content):
                content += s.recv(8196)
            break

    extension = file[file.rfind("_")+1:file.find(".")]
    if extension == "dir":
        extension = "dir.txt"
    file1 = open(os.path.join(filesDirectory, "recieved." + extension), "wb")
    file1.write(content)

s.sendall(bytes("GET / HTTP/1.1\r\nConnection: close\r\n\r\n", "utf-8"))
