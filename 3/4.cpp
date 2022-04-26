#include <stdio.h>
#include <sys/socket.h> // socket
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <stdlib.h>
#include <fcntl.h> //open
#include <unistd.h> //read,write
#include <string.h> //memset
#include <stdbool.h>
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <climits> // INT_MAX = 2147483647
#include <cstring> //.c_str()
// g++ -std=c++17 -pedantic -Wall 4.cpp -o server.o



int check_overflow_int(std::string number){
    /*
        Sprawdza czy wystąpi overflow przy konwersji.
        Return:
        -1    - jeśli wystąpi overflow
        -2    - jeśli podany znak jest niepoprawny
        wynik - jeśli nie wystąpi overflow
        

    */
    try {
        int x = std::stoi(number);
        return x;
    } catch (std::out_of_range  &e) {
        return -1;
    } catch (std::invalid_argument  &e) {
        return -2;
    }
}

// unsigned long check_overflow_long(std::string number){
//     try {
//         unsigned long x = std::stoul(number);
//         return x;
//     } catch (std::out_of_range  &e) {
//         return -1;
//     }
// }

// unsigned long long check_overflow_long_long(std::string number){
//     try {
//         unsigned long long x = std::stoull(number);
//         return x;
//     } catch (std::out_of_range  &e) {
//         return -1;
//     }
// }

int get_numbers(std::string numbers, std::vector<int> &vector_of_numbers){
    
    /*
        Funkcja konwertująca ciąg liczb w formie stringa na
        vector intów tych liczb

        Przyjmuje:
        - ciąg liczb w formie string
        - wynikowy vector

        Zwraca:
        - -1 - jeśli wystąpił ERROR
        - 1 - jeśli były tylko liczby
    */

    std::string tmp = "";
    
    int x, i;
    
    for(i=0; i<(int)numbers.length(); i++){
        // std::cout<<"-------------------------------------------"<<std::endl;
        // std::cout<<"i: "<< i << " literka: "<<numbers[i]<<std::endl;
        if( numbers[i] == ' '){
            x = check_overflow_int(tmp);
            if (x != -1 && x != -2){
                // int number = std::stoi(tmp);
                vector_of_numbers.push_back(x);
            } 
            else if (x == -2) continue;
            else if (x == -1) {
                vector_of_numbers.clear();
                break;
            }
            tmp = "";
        } else {
            if(isdigit(numbers[i]))
                tmp.push_back(numbers[i]);
        }   
    }
    x = check_overflow_int(tmp);
    if( x != -1 && x != -2){
        vector_of_numbers.push_back(x);
    }
    // for (int i : vector_of_numbers){
    //     std::cout<< i;
    // }
    // std::cout<<std::endl;

    if (numbers.find("ERROR") != std::string::npos){ //Error jest w stringu
        return -1;                  
    } 
    return 1;
}

int check_overflow_add(int first, int second){

    /*
        Sprawdza czy wystąpi overflow przy dodawaniu.
        Return:
        -1    - jeśli wystąpi overflow
        wynik - jeśli nie wystąpi overflow
        

    */

    if (first > INT_MAX - second)
        return -1;
    else
        return first + second;
}



bool is_correct_data(char* buf){

    /*
        Sprawdza czy podane dane są zgodne z założeniami.
        Return:
        true   - jeśli wystąpi są zgodne
        false  - jeśli nie są zgodne
        

    */
    // for(int i=0; i<=(sizeof((char*)buf)/sizeof(char*)) + 20 ; i++){
    //     std::cout<< buf[i]<<" ";
    // }
    std::string s((char*)buf);
    std::string possible_chars("0123456789 ");
    if (s.length() == 0 ) return false;
    for(char const &c : s){
        if( possible_chars.find(c) != std::string::npos ||
            c == 10 || c == 13 ){
            continue;
        } else {
            return false;
        }
    }
    return true;
    
}

int main(int argc, char *argv[]){

    int sock_fd;
    struct sockaddr_in server_address, client_address;
    

    // SOCK_DGRAM - UDP connection, 0 - default
    if( (sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ){
        perror("socket() failed\n");
        exit(1);
    } else {
        printf("Socket created...\n");
    }
    
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(2020);


    char buff[65535]; // max datagram size in udp
    // system("kill -9 $(sudo lsof -t -i:8080)");
    // system("lsof -t -i:8080");
    if (bind(sock_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0){
        perror("bind() failed\n");
        exit(3);
    } else {
        printf("Server binded...\n");
    }
    int amount_of_bytes;
    socklen_t client_address_len = sizeof(client_address);
    while(1){
        memset(buff, 0,sizeof(buff));
        if ((amount_of_bytes = recvfrom(sock_fd, buff, sizeof(buff), 0, 
            (struct sockaddr *) &client_address, &client_address_len)) == -1)
        {
            
            perror("Server recvfrom() failed: ");
            exit(2);
        } else {
            bool flag = false;
            for(int i=0; i<= amount_of_bytes -2; i++){
                if((int)buff[i] == 0) flag = true; 
            }
            

            printf("Recevied message: %s (%d)\n", buff, amount_of_bytes);

            
            int result = 0;             // zmienna wyniku
            std::string message = "";   // wysyłany komunikat
            if(is_correct_data(buff)){
                std::string numbers(buff);      // konwertujemy na char* na string
                std::vector<int> vector_of_numbers; // bufor przechowujący wszystkie liczby
                int no_error_ocured = get_numbers(numbers, vector_of_numbers);  // wypełniamy bufor
                                                                                // i sprawdzamy czy 
                                                                                // wystąpił ERROR
                // int result = 0;
                int tmp;
                // std::cout<<"Rozmiar:"<<vector_of_numbers.size()<<std::endl;
                if(vector_of_numbers.size() != 0){  // jeśli bufor nie jest pusty
                    for(int numb : vector_of_numbers){
                        // std::cout<<"numb: "<<numb<<std::endl;
                        if( (tmp = check_overflow_add(result, numb)) != -1){    // sprawdzamy czy wystąpi
                                                                                // overflow po konwersji
                            result = tmp;
                        } else { // jeśli wystąpił overflow                     

                            // std::cout<<"Overflow error"<<std::endl;
                            result = -1;
                            break;
                        }
                    }
                    
                   
                } else {            // wystąpił błąd, brak wartości
                    result = -2;
                }
                
                
                if(result != -1 && result != -2){       
                    if (no_error_ocured == 1){
                        std::cout<<"Result: "<<result<<std::endl;
                        // message = "Result: " + std::to_string(result);
                        message = std::to_string(result);
                    } else {
                        std::cout<<"Result (until an error occurs): "<<result<<std::endl;
                        // message = "Result(until an error occurs): " + std::to_string(result);
                        message = std::to_string(result);
                    }
                    // std::cout<<"ERROR occured: "<<(no_error_ocured == 1 ? "False" : "True")<<std::endl;
                } else if (result == -1) {
                    std::cout<<"Overflow error!"<<std::endl;
                    // message = "Overflow error!";
                    message = "ERROR";
                } else if (result == -2) {
                    std::cout<<"Empty value!"<<std::endl;
                    // message = "Non valid data!";
                    message = "ERROR";
                }

            } else {
                std::cout<<"Non valid data"<<std::endl;
                // message = "Non valid data";
                message = "ERROR";
            }
            // message += "\n";
            // std::cout<<"message: "<<message<<std::endl;
            if ( flag == true ){
                message = "ERROR";
            }
            const char* hello = message.c_str(); // konwertujemy string na const char*
            if ((amount_of_bytes = sendto(sock_fd, hello, strlen(hello), 0,     //wysyłamy do
            (struct sockaddr *)&client_address, client_address_len)) == -1){

                perror("Server sendto() failed\n");
                exit(3);
            } else {
                printf("Data has been sent successfully!(%d)\n", amount_of_bytes);
           
            }

            

        }

        sleep(1);
        

    }

    if(close(sock_fd) == -1){
        perror("close() failed\n");

    }
}