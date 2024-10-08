// Simulate Go-Back-N ARQ protocol using TCP client
//CS22B1093 ROHAN G
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <time.h>
#include <ctype.h>

#define PORT 8080
#define MAX_BUFFER 1024
#define TIMEOUT 3  // Timeout duration in seconds
#define PACKET_LOSS_PROBABILITY 20 // 20% chance of packet loss or corruption

int simulatePacketLossOrCorruption() {
    return rand() % 100 < PACKET_LOSS_PROBABILITY;
}

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[MAX_BUFFER] = {0};
    char ack[MAX_BUFFER] = {0};
    int totalPackets, windowSize;
    int base = 0, nextSeqNum = 0;  // Go-Back-N window variables
    int ackNum = 0;

    srand(time(0));  // Seed for random number generation

    // Create socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nSocket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection failed\n");
        return -1;
    }

    printf("Enter the total number of packets to be transmitted: ");
    scanf("%d", &totalPackets);
    printf("Enter the window size: ");
    scanf("%d", &windowSize);

    while (base < totalPackets) {
        // Send packets within window
        while (nextSeqNum < base + windowSize && nextSeqNum < totalPackets) {
            snprintf(buffer, MAX_BUFFER, "Packet %d", nextSeqNum);
            printf("\nSending packet %d\n", nextSeqNum);
            send(sock, buffer, strlen(buffer), 0);
            nextSeqNum++;
        }

        // Wait for ACK or timeout
        fd_set readfds;
        struct timeval tv;
        int retval;

        tv.tv_sec = TIMEOUT;
        tv.tv_usec = 0;
        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);

        retval = select(sock + 1, &readfds, NULL, NULL, &tv);

        if (retval == -1) {
            perror("select() error");
            close(sock);
            return -1;
        } else if (retval == 0) {
            // Timeout occurred
            printf("Timeout: Retransmitting from packet %d...\n", base);
            nextSeqNum = base;  // Go back to base for retransmission
        } else {
            // Read ACK from receiver
            memset(ack, 0, MAX_BUFFER);
            valread = read(sock, ack, MAX_BUFFER);
            if (valread > 0) {
                ackNum = atoi(ack);
                if (ackNum >= base) {
                    printf("ACK received for packet %d\n", ackNum);
                    base = ackNum + 1;  // Slide window forward
                }
            }
        }
    }
    printf("\nAll packets transmitted successfully.\n");
    close(sock);
    return 0;
}
