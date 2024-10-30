// CS22B1093 ROHAN G

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

const char* classify_ip(const char* ip) {
    unsigned char a[4];
    sscanf(ip, "%hhu.%hhu.%hhu.%hhu", &a[0], &a[1], &a[2], &a[3]);

    if (a[0] >= 0 && a[0] <= 127) return "Class A";
    else if (a[0] >= 128 && a[0] <= 191) return "Class B";
    else if (a[0] >= 192 && a[0] <= 223) return "Class C";
    else if (a[0] >= 224 && a[0] <= 239) return "Class D";
    else if (a[0] >= 240 && a[0] <= 255) return "Class E";
    return "Unknown Class";
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    char* client_ip = inet_ntoa(address.sin_addr);
    printf("Client IP: %s\n", client_ip);

    const char* classification = classify_ip(client_ip);

    char response[BUFFER_SIZE];
    snprintf(response, sizeof(response), "IP Address: %s, Classification: %s", client_ip, classification);
    send(new_socket, response, strlen(response), 0);
    printf("Response sent: %s\n", response);

    close(new_socket);
    close(server_fd);
    return 0;
}