// client.c
// CS22B1093 ROHAN G
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <time.h>

#define PORT 8080
#define MAX_BUFFER 1024
#define TIMEOUT 3  // Timeout duration in seconds

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[MAX_BUFFER] = {0};
    char ack[MAX_BUFFER] = {0};
    int totalPackets, packetNumber = 1;
    
    srand(time(0));  // Seed for random number generation

    // Create socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nSocket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported\n");
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection failed\n");
        return -1;
    }

    // Get the total number of packets to be sent
    printf("Enter the total number of packets to be transmitted: ");
    scanf("%d", &totalPackets);

    while (packetNumber <= totalPackets) {
        // Create a packet message
        snprintf(buffer, MAX_BUFFER, "Packet %d", packetNumber);
        printf("\nSending packet %d: %s\n", packetNumber, buffer);
        
        // Send the packet to the server
        send(sock, buffer, strlen(buffer), 0);
        
        fd_set readfds;
        struct timeval tv;
        int retval;

        // Initialize the timeout structure
        tv.tv_sec = TIMEOUT;
        tv.tv_usec = 0;

        // Set up the file descriptor set
        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);

        // Wait for the ACK from the server or timeout
        retval = select(sock + 1, &readfds, NULL, NULL, &tv);

        if (retval == -1) {
            perror("select() error");
            close(sock);
            return -1;
        } else if (retval == 0) {
            printf("Timeout: ACK for packet %d not received. Retransmitting...\n", packetNumber);
            continue; // Retransmit the same packet
        } else {
            // ACK received
            memset(ack, 0, MAX_BUFFER);
            valread = read(sock, ack, MAX_BUFFER);
            if (valread > 0) {
                if (strcmp(ack, "ACK") == 0) {
                    printf("ACK received for packet %d.\n", packetNumber);
                    packetNumber++; // Move to the next packet
                } else if (strcmp(ack, "CORRUPTED_ACK") == 0) {
                    printf("Corrupted ACK received for packet %d. Retransmitting...\n", packetNumber);
                } else {
                    printf("No ACK or incorrect ACK received. Retransmitting packet %d...\n", packetNumber);
                }
            } else {
                printf("Error receiving ACK. Retransmitting packet %d...\n", packetNumber);
            }
        }
    }

    printf("\nAll packets transmitted successfully.\n");
    close(sock);
    return 0;
}
