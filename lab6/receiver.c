// CS22B1093 - ROHAN G

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
    int sock = 0;
    char buffer[BUFFER_SIZE] = {0};
    char filename[BUFFER_SIZE] = {0};

    // Create a socket file descriptor
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define the server address
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("TCP Client is running...\n");

    // Send message to server
    printf("Enter the filename to request (with extension): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0; // Remove newline

    printf("\nRequesting file from server...\n");
    send(sock, filename, strlen(filename), 0);
    
    // Read server's response
    int bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received <= 0) {
        perror("Error receiving server response");
        close(sock);
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0';
    printf("Server response: %s\n", buffer);

    if (strncmp(buffer, "File exist in the server", 24) == 0) {
        // Create new filename with '_recv' suffix
        char *dot = strrchr(filename, '.');
        char new_filename[BUFFER_SIZE];

        if (dot != NULL) {
            strncpy(new_filename, filename, dot - filename);
            new_filename[dot - filename] = '\0';
            strcat(new_filename, "_recv");
            strcat(new_filename, dot);
        } else {
            strcpy(new_filename, filename);
            strcat(new_filename, "_recv");
        }

        FILE *fp = fopen(new_filename, "wb");
        if (fp == NULL) {
            perror("Error creating new file");
            close(sock);
            exit(EXIT_FAILURE);
        }

        // Receive file contents from the server
        size_t total_bytes_received = 0;
        while ((bytes_received = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
            fwrite(buffer, 1, bytes_received, fp);
            total_bytes_received += bytes_received;
        }

        fclose(fp);
        printf("File '%s' received and saved. Total bytes received: %zu\n", new_filename, total_bytes_received);
    } else {
        printf("File does not exist on the server or an error occurred.\n");
    }

    // Close the socket
    close(sock);
    return 0;
}