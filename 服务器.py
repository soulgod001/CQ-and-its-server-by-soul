import socket
HOST='172.21.0.14'
PORT=80
s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.bind((HOST,PORT))
s.listen(5)
conn,addr = s.accept()
data=conn.recv(10240).decode('gbk')
print(data)
conn.close()
