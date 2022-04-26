#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open
#include <unistd.h> //read,write
#include <string.h> //memset
#define BYTES_AMOUNT 4

int main(int argc, char *argv[]){


    int file_in, file_out;
    if((file_in = open(argv[1], O_RDONLY, 0644)) == -1) {
        perror("Blad otwarcia pliku do odczytu\n");
        exit(EXIT_FAILURE);
    }
    

    if((file_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
        perror("Blad otwarcia pliku do zapisu");
        exit(EXIT_FAILURE);
    }

    char buff[BYTES_AMOUNT];    // bufor do odczytu/zapisu
    ssize_t read_bytes, write_bytes; // ilosc odczytanych/zapisanych bajtow
    
    while(read_bytes = read(file_in, buff, BYTES_AMOUNT) != 0){
        if ( read_bytes == -1 )
        {                                       //obsluga bledow
            perror("Problem z odczytem");
            exit(EXIT_FAILURE);
        }

        if( (write_bytes = write(file_out, buff, strlen(buff))) == -1){
            perror("Problem z zapisem");
            exit(EXIT_FAILURE);
        }

        memset(buff, 0, sizeof(buff)); // czyszczenie zawartosci tablicy
        
    }

    if (close(file_in) == -1 || close(file_out) == -1 )      
    {
        perror("close error");              //obsluga bledow
        exit(EXIT_FAILURE);
    }
}