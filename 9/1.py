import sys
import requests
# import os

# CURRENT_DIR = os.getcwd()
FILENAME = "strona.html"
# PATH = os.path.join(CURRENT_DIR, FILENAME)

def check_response(adress: str, filename: str) -> bool:
    try:
        request = requests.get(adress)

        if request.status_code != 200:
            sys.exit(1)
        if request.headers['Content-Type'] != 'text/html':
            sys.exit(2)
        if b"Institute of Theoretical Physics" not in request.content:
            sys.exit(3)
    except requests.exceptions.ConnectionError:
        sys.exit(4)
        
    with open(filename, 'wb') as f:
        f.write(request.content)
    
    sys.exit(0)

check_response("http://th.if.uj.edu.pl/", FILENAME)