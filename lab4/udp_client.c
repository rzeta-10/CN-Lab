// CS22B1093 ROHAN G

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in serv_addr;
    char *message = "Hello from client";
    char buffer[BUFFER_SIZE];

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Send message to the server
    sendto(sockfd, message, strlen(message), 0, (const struct sockaddr *)&serv_addr, sizeof(serv_addr));
    printf("Message sent to server\n");

    // Receive response from the server
    socklen_t addr_len = sizeof(serv_addr);
    int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&serv_addr, &addr_len);
    if (n < 0) {
        perror("Receive failed");
    } else {
        buffer[n] = '\0'; // Null-terminate the received string
        printf("Server: %s\n", buffer);
    }

    // Close the socket
    close(sockfd);
    return 0;
}
