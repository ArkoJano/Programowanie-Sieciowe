#include <stdio.h>
#include <sys/socket.h> // socket
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <stdlib.h>
#include <fcntl.h> //open
#include <unistd.h> //read,write
#include <string.h> //memset
#include <stdbool.h>

int main(int argc, char *argv[]){

    int sock_fd;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len;

    // SOCK_DGRAM - UDP connection, 0 - default
    if( (sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ){
        perror("socket() failed\n");
        exit(1);
    } else {
        printf("Socket created...\n");
    }

    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(2000);

    char* hello = "Hello world form server!\n";
    char buff[1024];

    if (bind(sock_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0){
        perror("bind() failed\n");
        exit(3);
    } else {
        printf("Server binded...\n");
    }
    int amount_of_bytes;
    while(1){
        memset(buff, 0,sizeof(buff));
        if ((amount_of_bytes = recvfrom(sock_fd, buff, sizeof(buff), 0, 
            (struct sockaddr *) &client_address, &client_address_len)) == -1)
        {
            perror("Server recvfrom() failed\n");
            exit(2);
        } else {
            
            printf("Recevied message: %s (%d)\n", buff, amount_of_bytes);
        }
        
        if ((amount_of_bytes = sendto(sock_fd, hello, strlen(hello), 0, 
        (struct sockaddr *)&client_address, client_address_len)) == -1){

            perror("Server sendto() failed\n");
            exit(3);
        } else {
            printf("Data has been sent successfully!(%d)\n", amount_of_bytes);
        }
        sleep(1);
        

    }

    if(close(sock_fd) == -1){
        perror("close() failed\n");

    }
}