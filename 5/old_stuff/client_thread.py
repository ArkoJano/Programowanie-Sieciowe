import threading
import socket

RECV_BUFFER = 1024      # wielkosc wysylanego komunikatu

class ClientThread(threading.Thread):

    def __init__(self, HOST, PORT, message):
        super().__init__()
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client_socket.connect((HOST, PORT))
        self.message = message
        
    def send_message(self, message):
        self.client_socket.send(bytes(message, 'utf-8'))
        try:
            received = self.client_socket.recv(RECV_BUFFER)
            return received
        except socket.error as e:
            print("Failed to receive message!")
    
    def run(self):
        return self.send_message(self.message)
    
    def __del__(self):
        self.client_socket.close()

