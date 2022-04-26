#include <stdio.h>
#include <sys/socket.h> // socket
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <stdlib.h>
#include <fcntl.h> //open
#include <unistd.h> //read,write
#include <string.h> //memset

#define MAX_CLIENTS 10

int main(int argc, char *argv[]){
    
    if( argc != 2){
        perror("You must pass only on port arg!");
        exit(1);
    }
    
    int serv_fd, client_fd;
    struct sockaddr_in serv_address;
    char hello[] = "Hello, world!\r\n";

    //AF_INET - IPv4, SOCK_STREAM - , 0 - wybranie domyslnego protokolu(TCP)
    if((serv_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket() failed");
        exit(2);
    } else {
        printf("Socket created...\n");
    }

    serv_address.sin_family = AF_INET;
    // INADDR_ANY - rozumiane jako 0.0.0.0
    serv_address.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_address.sin_port = htons(atoi(argv[1]));

    if (bind(serv_fd, (struct sockaddr *)&serv_address, sizeof(serv_address)) < 0){
        perror("bind() failed");
        exit(3);
    } else {
        printf("Server binded...\n");
    }
    
    if( listen(serv_fd, MAX_CLIENTS) != 0){
        perror("listen() failed");
        exit(4);
    } else {
        printf("Server listening...\n");
    }

    while(1){

        if ((client_fd = accept(serv_fd, NULL, 0)) == -1){
            perror("accept() failed");
            exit(7);
        } else {
            printf("Client accepted!");
        }
        
        if (write(client_fd, hello, sizeof(char)*strlen(hello)) != (sizeof(char)*strlen(hello)))
        {
            perror("write() failed");
            exit(5);
        } 

        if (close(client_fd) == -1){
            perror("close() failed");
            exit(6);
        }

    }

}