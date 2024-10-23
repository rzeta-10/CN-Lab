// Client Code

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
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    char filename[BUFFER_SIZE];

    // Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define the server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("TCP Client is running...\n");
    printf("Enter the filename to request from the server: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0; // Remove newline character

    // Send the filename to the server
    send(sock, filename, strlen(filename), 0);
    printf("Requested file: %s\n", filename);

    // Read the server's response
    int bytes_received = read(sock, buffer, BUFFER_SIZE);
    buffer[bytes_received] = '\0'; // Null-terminate the string

    // Check if it's an error message
    if (strncmp(buffer, "Error: File does not exist", 26) == 0) {
        printf("%s\n", buffer);
    } else {
        // It's a valid file, so save it locally
        FILE *file = fopen(filename, "wb");
        if (file == NULL) {
            perror("Error creating file");
            close(sock);
            exit(EXIT_FAILURE);
        }

        printf("Receiving file...\n");
        // Write the received data to the file
        do {
            fwrite(buffer, 1, bytes_received, file);
            bytes_received = read(sock, buffer, BUFFER_SIZE);
        } while (bytes_received > 0);

        fclose(file);
        printf("File '%s' received successfully.\n", filename);

        // Display the content of the received file
        file = fopen(filename, "r");
        if (file) {
            printf("Content of '%s':\n", filename);
            while (fgets(buffer, sizeof(buffer), file) != NULL) {
                printf("%s", buffer);
            }
            fclose(file);
        }
    }

    // Close the socket
    close(sock);
    return 0;
}
