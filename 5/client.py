import socket

RECV_BUFFER = 1024      # wielkosc wysylanego komunikatu

class Client(object):

    def __init__(self, HOST, PORT):
        self.client_socket  = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client_socket.connect((HOST, PORT))

    def send_message(self, message):
        if type(message) == type(""):
            self.client_socket.send(bytes(message, 'utf-8'))
        elif type(message) == type(bytes()):
            self.client_socket.send(message)
            
        try:
            received = self.client_socket.recv(RECV_BUFFER)
            return received
        except socket.error as e:
            print("Failed to receive message!")


    def __del__(self):
        self.client_socket.close()

client = Client("127.0.0.1", 2020)
message = b'12 12 12'
print("Received:  ", client.send_message(message))