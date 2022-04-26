import unittest
from client import Client
import threading
import concurrent.futures
from termcolor import colored
import time

HOST = '127.0.0.1'
PORT = 2020
AMOUNT_OF_THREADS = 3
TIMEOUT = 2

DATA = {
    # "55 55\r\n"             :   b'110\r\n',
    # "random words\r\n"      :   b'ERROR\r\n',
    # "55\r\n"                :   b'55\r\n',
    # "\r\n"                  :   b'ERROR\r\n',
    # "55 55\r\n 55 55\r\n"   :   b'110\r\nERROR\r\n',
    # "\r\n\r\n\r\n"          :   b'ERROR\r\nERROR\r\nERROR\r\n',
    # "55 55\r 33 44\r\n"     :   b'ERROR\r\n',
    # "55 55\n 33 44\r\n"     :   b'ERROR\r\n',
    # "55 55\0\r\n"           :   b'ERROR\r\n',
    # "55  55\r\n"            :   b'ERROR\r\n',
    # "%^&*()!@#$\r\n"        :   b'ERROR\r\n',
    # b"binnary\r\n"          :   b'ERROR\r\n',
    # b"12 22\r\n55 55\r\n"   :   b'34\r\n110\r\n',
    b"12 22"                :   b'',


}

class TestSumServer(unittest.TestCase):

    def setUp(self):
        pass


    def test_single_client(self):
        print(colored("------------------ TEST_SINGLE_CLIENT ------------------", "red"))
        self.client = Client(HOST, PORT)
        for i , (key, value) in enumerate(DATA.items()):
            result = self.client.send_message(key)
            
            try:
                self.assertEqual(result, value)
                print(f'[{i+1}] {colored("SUCCESS", "green")}')

            except AssertionError as e:
                
                print(f'[{i+1}] {colored("FAILED", "red")}')
                print(f"SENDED: {key}\n")
                print(f"EXPECTED: {value}\n")
                print(f"RECEIVED: {result}\n")
                # raise e
        del self.client

    def test_multiple_clients(self):
        print(colored("------------------ TEST_MULTIPLES_CLIENT ------------------", "red"))
        with concurrent.futures.ThreadPoolExecutor(max_workers=AMOUNT_OF_THREADS) as executor:

            for i, (key, value) in enumerate(DATA.items()):
                for _ in range(AMOUNT_OF_THREADS):
                    client = Client(HOST, PORT)
                    future_obj = (executor.submit(client.send_message, key))
                    
                    try:
                        self.assertEqual(future_obj.result(), value)
                        print(f'[{i+1}] {colored("SUCCESS", "green")}')
                    except AssertionError as e:
                        print(f'[{i+1}] {colored("FAILED", "red")}')
                        print(f"SENDED: {key}\n")
                        print(f"EXPECTED: {value}\n")
                        print(f"RECEIVED: {future_obj.result()}\n")
                        raise e

    def test_single_client_timeout(self):
        print(colored("------------------ TEST_SINGLE_CLIENT_TIMEOUT ------------------", "red"))
        self.client = Client(HOST, PORT)
        for i , (key, value) in enumerate(DATA.items()):
            try:
                time.sleep(TIMEOUT)
            except KeyboardInterrupt:
                pass
            result = self.client.send_message(key)
            
            try:
                self.assertEqual(result, value)
                print(f'[{i+1}] {colored("SUCCESS", "green")}')

            except AssertionError as e:
                
                print(f'[{i+1}] {colored("FAILED", "red")}')
                print(f"SENDED: {key}\n")
                print(f"EXPECTED: {value}\n")
                print(f"RECEIVED: {result}\n")
                # raise e
        del self.client

    def test_multiple_clients_timeout(self):
        print(colored("------------------ TEST_MULTIPLE_CLIENTS_TIMEOUT ------------------", "red"))
        with concurrent.futures.ThreadPoolExecutor(max_workers=AMOUNT_OF_THREADS) as executor:

            for i, (key, value) in enumerate(DATA.items()):
                for _ in range(AMOUNT_OF_THREADS):
                    client = Client(HOST, PORT)
                    time.sleep(TIMEOUT)
                    future_obj = (executor.submit(client.send_message, key))
                    
                    try:
                        try:
                            self.assertEqual(future_obj.result(), value)
                            print(f'[{i+1}] {colored("SUCCESS", "green")}')
                        except concurrent.futures._base.TimeoutError:
                            print(f'[{i+1}] {colored("TIMEOUT", "white")}')

                    except AssertionError as e:
                        print(f'[{i+1}] {colored("FAILED", "red")}')
                        print(f"SENDED: {key}\n")
                        print(f"EXPECTED: {value}\n")
                        print(f"RECEIVED: {future_obj.result()}\n")
                        raise e

    def tearDown(self):
        pass


if __name__ == '__main__':
    unittest.main()
