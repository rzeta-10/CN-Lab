//CS22B1093 ROHAN G

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int main()
{
    int server_fd, new_socket;

    char buffer[1024] = {0};
    char *hello = "Hello from server";

    //create a socket file descriptor
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //define the server address
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    socklen_t addrlen = sizeof(address);

    //bind the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    //listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("TCP Server is running and waiting for messages...\n");

    //accept connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);

    if(new_socket < 0) {
        perror("Connection acceptance failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    //read message from client
    read(new_socket, buffer, 1024);
    printf("Client: %s\n", buffer);

    //send message to client
    send(new_socket, hello, strlen(hello), 0);
    printf("Message sent to client\n");

    //close the socket
    close(new_socket);
    close(server_fd);

}