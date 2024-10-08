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
    char buffer[BUFFER_SIZE];
    
    struct sockaddr_in serv_addr, client_addr;
    socklen_t addr_len;

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Define server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;         // IPv4
    serv_addr.sin_addr.s_addr = INADDR_ANY; // Accept any IP
    serv_addr.sin_port = htons(PORT);       // Port

    // Bind the socket to the server address
    if (bind(sockfd, (const struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP Server is running and waiting for messages...\n");

    addr_len = sizeof(client_addr);

    // Receive data from the client
    int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
    if (n < 0) {
        perror("Receive failed");
    } else {
        buffer[n] = '\0'; // Null-terminate the received data
        printf("Client: %s\n", buffer);

        // Respond to the client
        char *response = "Hello from server";
        sendto(sockfd, response, strlen(response), 0, (struct sockaddr *)&client_addr, addr_len);
        printf("Response sent to client\n");
    }

    // Close the socket
    close(sockfd);
    return 0;
}
