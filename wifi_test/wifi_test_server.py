import socket
import time

soc = socket.socket(socket.AF_INET)
soc.bind(("0.0.0.0", 3233))
soc.listen()

client, addr = soc.accept()
print(f"Client Addr: {addr}")
for i in range(100):
    text = f'No: {i}\n'
    client.send(bytes(text, encoding='utf-8'))
    time.sleep(1)
