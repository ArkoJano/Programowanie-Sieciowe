#include <stdio.h>
#include <sys/socket.h> // socket
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <stdlib.h>
#include <fcntl.h> //open
#include <unistd.h> //read,write
#include <string.h> //memset
#include <stdbool.h>
#include <arpa/inet.h> // warning "implicit declaration of function 'inet_addr'"


bool drukowalne(const void * buf){
    int i = 0;
    const char* char_buf = buf;
    // for(i=0; i<sizeof(char_buf); i++){
    //     if(char_buf[i]<32 || char_buf[i]>126){
    //         return false;
    //     }
    // }

    // printf("%c", char_buf[i]);
    while(char_buf[i] != '\0'){

        if(char_buf[i]<32 || char_buf[i]>126){
            return false;
        }
        i++;
    }
    return true;
}
void drukuj(const void * buf){
    int i = 0;
    const char* char_buf = buf;
    
    while(char_buf[i] != '\0'){
        if(char_buf[i]>=32 && char_buf[i]<=126){
            printf("%c", char_buf[i]);
        }
        i++;
    }
    
}

int main(int argc, char *argv[]){
    if( argc != 3){
        perror("You must pass IPv4 and TCP port in args!");
        exit(1);
    }

    int sock_fd;
    struct sockaddr_in serv_address;

    if( (sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
        perror("socket() failed");
        exit(2);
    } else {
        printf("Socket created...\n");
    }

    memset(&serv_address, '0', sizeof(serv_address)); 

    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(atoi(argv[2]));
    serv_address.sin_addr.s_addr = inet_addr(argv[1]);
    // if(inet_pton(AF_INET, argv[1], &serv_address.sin_addr)<=0)
    // {
    //     printf("Adress writing failed\n");
    //     return 1;
    // } 

    if( connect(sock_fd, (struct sockaddr *)&serv_address, sizeof(serv_address)) != 0){
        perror("connect() failed");
        exit(3);
    } else {
        printf("Connected!\n");
    }

    char buff[100];

    sleep(1);  
    read(sock_fd, buff, 100);
    // if(drukowalne(buff)){
    //     printf("%s", buff);
    // }

    drukuj(buff);

    // while(read(sock_fd, buff, 10) > 0){
        // if(drukowalne(buff)){
            // printf("%s", buff);
        // }
    // }
        
    if (close(sock_fd) == -1){
        perror("close() failed");
        exit(6);
    }
}
