import re
import socket
from _thread import *

IP_ADDR = "127.0.0.1"
PORT = 2020
ACCEPTABLE_CHARACTERS = ["123456789 "]

def client_thread(client, client_addr):
    print(f"{client_addr[0]}:{client} | Connected!")
    result_message = ""
    error = False
    bufor_for_one_number = ""
    result = 0
    while True:
        counter = 1
        recevied_data = client.recv(1024)
        if len(recevied_data) != 0:
        #     print(f"Recevied data: {recevied_data}")
        #     messages[f"#{counter}"] = {
        #             "result_message"       : "",
        #             "error"                : False,
        #             "result"               : 0
        #         }

        #     for index, i in enumerate(recevied_data):
        #         if i not in ACCEPTABLE_CHARACTERS and i != '\r' and i != '\n':
        #             # result_message = "ERROR"
        #             messages[f"#{counter}"]["error"] = True
        #             # break
        #         elif i == " ":
        #             if (bufor_for_one_number) != 0:
        #                 try:
        #                     messages[f"#{counter}"]["result"] += int(bufor_for_one_number)
        #                 except ValueError:
        #                     print("ERROR while converting to integer")
        #                     # result_message = "ERROR"
        #                     messages[f"#{counter}"]["error"] = True
        #                     # break
        #                 bufor_for_one_number = ""
        #             else:
        #                 continue
        #         else:
        #             bufor_for_one_number += i
        else:

        # message = ""
        message = str(recevied_data)
        if error == True:
            messages[f"#{counter}"]["result_message"] = "ERROR"
        
            for key, value in messages.items():
                message += f'{key} result: {messages[key]["result_message"]}'
        
        client.sendall(str.encode(message))
        # client.sendall(recevied_data)

messages = dict()

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    while True:
        try:
            s.bind((IP_ADDR, PORT))
        except socket.error as e:
            print(str(e))

        s.listen()

        client, client_addr = s.accept()
        start_new_thread(client_thread, (client, client_addr, ))
            # break
        