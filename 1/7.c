#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open
#include <unistd.h> //read,write
#include <string.h> //memset
#include <stdbool.h> // bool
#define BYTES_AMOUNT 1



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
    int counter_of_lines = 1;  // sprawdza parzystosc lini
    //bool end_of_line = false; // flaga mowiaca czy znajduje sie koniec linii
    //size_t amount_of_bytes_to_write;
    //int i=0;
    //char *tmp; // dodatkowy bufor

    while((read_bytes = read(file_in, buff, sizeof(buff))) != 0){
        if ( read_bytes == -1 )
        {                                       
            perror("Problem z odczytem");
            exit(EXIT_FAILURE);
        }

        
        // printf("Line: %d, %s\n", buff[0], buff);
        if(buff[0] == 10){
            counter_of_lines++;
            // printf("Counter: %d", counter_of_lines);
        }
        // for(i=0; i<strlen(buff); i++){
            // if(buff[i] == 10){      // jesli znajduje sie tu koniec linii
                // end_of_line = false;
                // tmp = NULL;
                // for(i=0; i<=strlen(buff); i++){
                //     if(buff[i] == '\n'){
                //         end_of_line = true;
                //         break;
                //     }
                // }

                // if(end_of_line == true){
                    
                //     if(counter_of_lines % 2 == 0){  // jesli linia jest parzysta
                //         tmp = strtok(buff, "\n");
                //         // if((tmp = strtok(buff, '\n')) != NULL){ // zostawia tylko to co ZA "\n"
                //         strcat(tmp, "\n");          // dodanie nowej linii do nowego tekstu
                //         // } 
                //     } else {
                    
                //         tmp = strtok(buff, "\n");  // zostawia tylko to co PO "\n"
                //         tmp = strtok(NULL, "\n");  // zostawia tylko to co PO "\n"
                // }  
                // }
                
                // if( tmp != NULL ){
                //     printf("Tmp: %s\n", tmp);
                //     printf("Buff: %s\n", buff);
                //     counter_of_lines++;

                // }
                // memset(buff, 0, sizeof(buff)); // czysczenie pamieci na nowy tekst
                // strcpy(buff, tmp);          // zapisanie nowej linii w buforze
            // } 
            
        // }

        // if(end_of_line == true){
        //     amount_of_bytes_to_write = i;
        // } else {
        //     amount_of_bytes_to_write = strlen(buff);
        // }

        if((counter_of_lines % 2) == 1){
            if( (write_bytes = write(file_out, buff, BYTES_AMOUNT)) == -1){
                perror("Problem z zapisem");
                exit(EXIT_FAILURE);
            }
        }
        
        // end_of_line = false;
        // memset(buff, 0, sizeof(buff)); // czyszczenie zawartosci tablicy
        
    }

    if (close(file_in) == -1 || close(file_out) == -1 )      
    {
        perror("close error");              //obsluga bledow
        exit(EXIT_FAILURE);
    }
}