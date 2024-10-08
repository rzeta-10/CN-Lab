// Receiver program for Go-Back-N protocol using TCP server
//CS22B1093 ROHAN G
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define MAX_BUFFER 1024
#define PACKET_LOSS_PROBABILITY 20 // 20% chance of packet loss or corruption

int simulatePacketLossOrCorruption() {
    return rand() % 100 < PACKET_LOSS_PROBABILITY;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX_BUFFER] = {0};
    int expectedSeqNum = 0;  // The next packet the receiver expects

    srand(time(0));  // Seed for random number generation

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Receiver is listening on port %d...\n", PORT);

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        memset(buffer, 0, MAX_BUFFER);
        int valread = read(new_socket, buffer, MAX_BUFFER);
        if (valread == 0) {
            printf("Connection closed by sender.\n");
            break;
        }

        printf("\nReceived: %s\n", buffer);

        if (simulatePacketLossOrCorruption()) {
            printf("Packet %d lost or corrupted. No ACK sent.\n", expectedSeqNum);
            continue;
        }

        int receivedSeqNum = atoi(buffer + 7);  // Extract packet number

        if (receivedSeqNum == expectedSeqNum) {
            printf("Packet %d received successfully.\n", receivedSeqNum);
            expectedSeqNum++;
        } else {
            printf("Out-of-sequence packet received. Expected packet %d.\n", expectedSeqNum);
        }

        char ack[MAX_BUFFER];
        snprintf(ack, MAX_BUFFER, "%d", expectedSeqNum - 1);
        printf("Sending cumulative ACK for packet %d\n", expectedSeqNum - 1);
        send(new_socket, ack, strlen(ack), 0);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
