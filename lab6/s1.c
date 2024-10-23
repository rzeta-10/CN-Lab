#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int server_fd, new_socket;
    char buffer[BUFFER_SIZE] = {0};
    char filename[BUFFER_SIZE] = {0};

    // Create a socket file descriptor
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Define the server address
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    socklen_t addrlen = sizeof(address);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("TCP Server is running and waiting for messages...\n");
    printf("TCP Server is running and waiting for messages...\n");
    
    FILE *file;
    // Ask the user for the filename
    printf("Enter the filename to create or write to (with extension): ");
    fgets(filename, sizeof(filename), stdin);

    // Remove newline character from the filename input
    filename[strcspn(filename, "\n")] = 0;

    // Open the file in write mode
    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    // Prompt the user to enter the content
    printf("Enter the content to write to the file (type 'STOP' to end input):\n");

    // Keep reading lines from the user until "STOP" is entered
    while (1) {
        fgets(buffer, sizeof(buffer), stdin);

        // Check if the user typed "STOP" (ignoring newline)
        if (strncmp(buffer, "STOP", 4) == 0 && buffer[4] == '\n') {
            break;
        }

        // Write the user's input to the file
        fprintf(file, "%s", buffer);
    }

    // Close the file
    fclose(file);

    printf("Content written to file '%s' successfully.\n", filename);

    while(1) {
        printf("Accepting client connections...\n");

        // Accept connection
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if(new_socket < 0) {
            perror("Connection acceptance failed");
            continue;
        }

        // Read message from client
        int bytes_received = recv(new_socket, filename, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            perror("Error receiving filename from client");
            close(new_socket);
            continue;
        }
        filename[bytes_received] = '\0';
        printf("Request of file from client: %s\n", filename);

        // Check if the file exists
        FILE *fp = fopen(filename, "rb");
        if (fp != NULL) {
            printf("File '%s' exists in the current directory.\n", filename);

            const char *msg = "File exist in the server";
            send(new_socket, msg, strlen(msg), 0);
            printf("Message sent to client\n");

            // Send file contents to the client
            while ((bytes_received = fread(buffer, 1, BUFFER_SIZE, fp)) > 0) {
                send(new_socket, buffer, bytes_received, 0);
            }
            fclose(fp);
        } else {
            printf("File '%s' does not exist in the current directory.\n", filename);
            const char *msg = "File does not exist in the server";
            send(new_socket, msg, strlen(msg), 0);
            printf("Message sent to client\n");
        }

        close(new_socket);
        break;
    }

    close(server_fd);
    return 0;
}