#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void random_backoff() {
    int delay = rand() % 5 + 1; // Random delay between 1 and 5 seconds
    printf("Backing off for %d seconds...\n", delay);
    sleep(delay);
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *data = "Data from Client";
    char buffer[BUFFER_SIZE] = {0};

    srand(time(0));

    while (1) {
        // Create socket
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("Socket creation error\n");
            return -1;
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
            printf("Invalid address\n");
            return -1;
        }

        // Connect to the server
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            printf("Connection failed\n");
            return -1;
        }

        // Check if medium is idle before sending
        send(sock, data, strlen(data), 0);
        printf("Attempting to transmit: %s\n", data);

        // Read the server response
        read(sock, buffer, BUFFER_SIZE);
        printf("Server response: %s\n", buffer);

        // If collision occurs, back off and retry
        if (strcmp(buffer, "COLLISION") == 0) {
            random_backoff();
        } else {
            printf("Transmission was successful.\n");
            break;
        }

        close(sock);
    }

    return 0;
}
