import socket
import time

soc = socket.socket(socket.AF_INET)
soc.connect(("8.217.197.89", 5663))
for i in range(100):
    soc.send(bytes(i))
    print(i)
data = soc.recv(1024)
print(data)
