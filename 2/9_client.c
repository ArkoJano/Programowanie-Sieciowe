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

int main(int argc, char *argv[]){

    if( argc != 2){
        perror("You must pass a message!");
        exit(1);
    }

    int sock_fd;
    struct sockaddr_in server_address;
    socklen_t len;

    // SOCK_DGRAM - UDP connection, 0 - default
    if( (sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ){
        perror("socket() failed");
        exit(1);
    } else {
        printf("Socket created...\n");
    }

    server_address.sin_addr.s_addr = inet_addr("0.0.0.0");
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(2000);

    // char* hello = "Hello world form client!\n";
    char* message = argv[1];
    char buff[65535];

    if (sendto(sock_fd, message, strlen(message), 0, 
       (struct sockaddr *)&server_address, sizeof(server_address)) == -1){

        perror("Client sendto() failed\n");
        exit(2);
    } else {
        printf("Data has been sent successfully!\n");
    }
    sleep(2);
    int amount_of_recevied_bytes;
    if (( amount_of_recevied_bytes = recvfrom(sock_fd, buff, sizeof(buff), 0, 
        (struct sockaddr *) &server_address, &len)) == -1)
    {
        perror("Client recvfrom() failed\n");
        exit(2);
    } else {

        printf("Recevied message: %s \n Bytes: %d\n", buff, amount_of_recevied_bytes);
    }

    if(close(sock_fd) == -1){
        perror("close() failed\n");
        exit(5);
    }

    return 0;

}