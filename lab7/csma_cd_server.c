#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
int medium_busy = 0; // 0: idle, 1: busy

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 4) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    printf("CSMA/CD Server is running...\n");

    while (1) {
        // Accept a new connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept");
            exit(EXIT_FAILURE);
        }

        // Read the incoming message
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        buffer[valread] = '\0'; // Null-terminate the buffer
        printf("Received request to transmit: %s\n", buffer);

        // Check for the exit condition
        if (strcmp(buffer, "EXIT") == 0) {
            printf("Exit signal received. Shutting down the server.\n");
            close(new_socket);
            break; // Exit the while loop
        }

        // Simulate transmission and collision detection
        if (medium_busy) {
            printf("Medium is busy. Collision detected!\n");
            send(new_socket, "COLLISION", strlen("COLLISION"), 0);
        } else {
            medium_busy = 1;
            printf("Medium is idle. Transmission successful.\n");
            send(new_socket, "SUCCESS", strlen("SUCCESS"), 0);
            sleep(2); // Simulate transmission time
            medium_busy = 0;
        }

        close(new_socket);
    }

    // Close the server socket
    close(server_fd);
    printf("Server closed.\n");

    return 0;
}
