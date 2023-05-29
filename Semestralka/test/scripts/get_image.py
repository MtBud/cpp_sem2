import socket

HOST = "127.0.0.1"  # The server's hostname or IP address
PORT = 8080  # The port used by the server

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    lines = "GET /images/good_book.jpg HTTP/1.1\r\nConnection: keep-alive\r\n\r\n"
    s.send(bytes(lines, "ascii"))
    data = s.recv(100000)
    image = open("recieved.jpeg", "w")
    image.write(str(data))
    image.close()

print(f"Received {data!r}")
