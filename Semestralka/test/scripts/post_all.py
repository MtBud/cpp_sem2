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

directory = "../post_requests"
filesDirectory = "../post_files"
for file in os.listdir(directory):
    requestFile = open(os.path.join(directory, file), "rb")
    requestFileContent = requestFile.read().decode()
    filePath = requestFileContent[requestFileContent.find("\r\n\r\n")+4:]
    fileName, extension = os.path.splitext(filePath)
    extension = extension[1:]

    bodyFile = open(os.path.join(filesDirectory, filePath), "rb")
    body = bodyFile.read()
    length = len(body)
    requestFileContent = requestFileContent[:requestFileContent.find("\r\n\r\n")]
    requestFileContent += ("\r\n" + "Content-Length: " + str(length) + "\r\n")
    requestFileContent += ("Content-Type: text/" + extension + "\r\n\r\n")
    message = bytes(requestFileContent, "utf-8") + body
    s.sendall(message)

    reply = s.recv(1024).decode()
    print("REPLY")
    print(reply)


# s.sendall(bytes("GET / HTTP/1.1\r\nConnection: close\r\n\r\n", "utf-8"))
