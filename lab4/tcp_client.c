//CS22B1093 ROHAN G

#include <arpa/inet.h> 
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int main()
{
    int sock = 0;
    
    char *message = "Hello from client";
    char buffer[1024] = {0};

    //create a socket file descriptor
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    //define the server address
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    
    //convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        close(sock);
        exit(EXIT_FAILURE);
    }

    //connect to the server
    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    //send message to server
    send(sock, message, strlen(message), 0);
    printf("Message sent to server\n");

    //read message from server
    read(sock, buffer, 1024);
    printf("Server: %s\n", buffer);

    //close the socket
    close(sock);
    return 0;
}