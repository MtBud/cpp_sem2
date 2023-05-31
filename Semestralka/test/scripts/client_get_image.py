import socket

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

s.sendall(bytes("GET /images/good_book.jpg HTTP/1.1\r\nConnection: close\r\n\r\n", "utf-8"))
size = 1
request = ""
content = bytearray()
while size == 1:
    newData = s.recv(1).decode()
    request += newData
    size = len(newData)
    if request.find("\r\n\r\n") != -1:
        print("request: ", request)
        print("find:", request.find("\r\n\r\n"))
        content += bytes(request[request.find("\r\n\r\n")+4:], "utf-8")
        size = 1024
        while size == 1024:
            newData = s.recv(1024)
            content += newData
            size = len(newData)


file1 = open("recieved.jpg", "wb")
file1.write(content)
