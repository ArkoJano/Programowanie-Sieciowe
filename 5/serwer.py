import socket
from _thread import *
import threading    # Thread
import codecs       # konwersja z \\r na \r
import time         # timeout

IP_ADDR = "0.0.0.0"
PORT = 2020
ACCEPTABLE_CHARACTERS = "0123456789"
CONNECTION_LIST = []	# lista socketow klienckich
RECV_BUFFER = 1024      # wielkosc obieranego komunikatu
TIMEOUT = 10            # timeout podawany w sekundach

def is_correct_data(recevied_data):
    print("Received data : ",recevied_data)
    tmp_list = list(recevied_data)
    if len(recevied_data) == 0:
        print("ERROR not valid lenght of received data")
        return -2
    elif recevied_data == "\r\n":
        print("ERROR \ r \ n only")
        return -1

    # komunikat nie zaczyna sie odpowiednim znakiem
    elif tmp_list[0] not in ACCEPTABLE_CHARACTERS: 
        print("ERROR not a valid characters: ", tmp_list[0])
        return -1

    # komunikat nie konczy sie odpowiednim znakiem
    elif tmp_list[-1] not in ACCEPTABLE_CHARACTERS:
        print("ERROR not a valid characters: ", tmp_list[-1])
        return -1

    return 1
    

def send_data(client, result, error):
    print("Sended :", bytes(f"{result}", encoding='ascii'))
    client.send(bytes(f"{result}", encoding='ascii'))

def get_data(client):
    recevied_data = list()
    while True:
        tmp = b''
        try:
            tmp = client.recv(RECV_BUFFER)
        except socket.timeout:
            pass
        except socket.error:
            pass
        
        if not tmp: break

        recevied_data.append(tmp)
        
        # k += 1

    data = ""
    for elem in recevied_data:

        # # dekodowanie \\r na \r
        elem = codecs.escape_decode(elem)
        data += elem[0].decode("utf-8")

    return data

def split_to_one(data):
    return data.split("\r\n")[:-1]

def client_thread(client, client_addr, timeout=10):
    print("----------------------------------------------------------------")
    print(f"{client_addr[0]} | Connected!")
    print("----------------------------------------------------------------")
    bufor_for_one_number = ""
    result = 0
    final_meesage = ""

    start_time = time.time()    # zmienna wyliczajaca timeout

    # dane beda odbierane tylko przez tyle czasu ile podano w argumencie
    while (time.time() - start_time) < timeout:

        data = get_data(client)

        if data == '':     # jesli nie odebrano niczego
            continue

        data = split_to_one(data)   # komunikaty rodzielane są \r\n

        for index, request in enumerate(data):
            
            correct = is_correct_data(request)  #sprawdzamy czy dane sa poprawne
            if (correct == 1):
                
                for i, number in enumerate(request):
                    
                    # jesli dochodzimy do konca komunikatu
                    # dodajemy reszte liczb, czyscimy bufor
                    # wysylamy wszystko do klienta

                    if number in ACCEPTABLE_CHARACTERS:
                        bufor_for_one_number += number
                    
                    
                    # jesli natrafilismy na spacje
                    elif number == ' ':
                        # sprawdzamy czy nastepny znak to spacja
                        try:
                            next_item =  list(request)[i+1]
                            if next_item == ' ':
                                final_meesage += f"ERROR\r\n"
                                print("ERROR double space")
                                break
                        except IndexError:
                            pass
                        
                            # konwertujemy na liczbę i dodajemy do wyniku
                            # czyszczac bufor
                        if bufor_for_one_number != '':
                            try:
                                result += int(bufor_for_one_number)
                                bufor_for_one_number = ''
                            except ValueError:
                                print("ERROR while converting to integer")
                                final_meesage += f"ERROR\r\n"
                                break
                    else:
                        final_meesage += f"ERROR\r\n"
                        break
                    
                    if i == len(request) - 1:
                        
                        if bufor_for_one_number != '':
                            try:
                                result += int(bufor_for_one_number)
                                bufor_for_one_number = ''
                            except ValueError:
                                print("ERROR while converting to integer")
                                final_meesage += f"ERROR\r\n"
                                break

                        final_meesage += f"{result}\r\n"
                        result = 0
                    
            elif correct != 1:
                final_meesage += f"ERROR\r\n"
                result = 0

        # print("final_meesage: ", final_meesage)
        send_data(client, final_meesage, True)
        final_meesage = ""

        start_time = time.time() # resetowanie timeout'u
    print("CLIENT CONNECTION: closed")
    client.close()     


messages = dict()



with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    list_of_threads = list()
    
    try:
        s.bind((IP_ADDR, PORT))
    except socket.error as e:
        print(str(e))

    s.listen()

    while True:
        client, client_addr = s.accept()
        client.setblocking(False) 
        new_tread = threading.Thread(target=client_thread, args=(client, client_addr, TIMEOUT, ))
        new_tread.start()
        # new_tread.join()
        

