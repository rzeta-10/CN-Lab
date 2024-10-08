// server.c
// CS22B1093 ROHAN G
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define MAX_BUFFER 1024
#define PACKET_LOSS_PROBABILITY 20 // 20% chance of packet loss or corruption

// Function to simulate packet loss or corruption
int simulatePacketLossOrCorruption() {
    // Generate a random number to simulate packet loss or corruption
    int randomValue = rand() % 100;
    return (randomValue < PACKET_LOSS_PROBABILITY);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX_BUFFER] = {0};
    char ack[] = "ACK";  // Normal acknowledgment
    char corruptedAck[] = "CORRUPTED_ACK";  // Simulated corrupted ACK
    
    srand(time(0));  // Seed for random number generation

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Server is listening on port %d...\n", PORT);
    
    // Accept the connection from the client
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    
    int packetNumber = 1;
    while (1) {
        memset(buffer, 0, MAX_BUFFER);
        // Read packet from client
        int valread = read(new_socket, buffer, MAX_BUFFER);
        
        if (valread == 0) {
            printf("Connection closed by client.\n");
            break;
        }
        
        printf("\nReceived packet %d: %s\n", packetNumber, buffer);

        // Simulate packet loss or corruption
        if (simulatePacketLossOrCorruption()) {
            int corruptPacket = rand() % 2; // Decide between loss or corruption
            if (corruptPacket == 0) {
                printf("Packet %d lost. No ACK sent.\n", packetNumber);
            } else {
                printf("Packet %d corrupted. Sending corrupted ACK...\n", packetNumber);
                send(new_socket, corruptedAck, strlen(corruptedAck), 0);  // Send corrupted ACK
            }
            packetNumber--;
        } else {
            printf("Packet %d received successfully. Sending ACK...\n", packetNumber);
            send(new_socket, ack, strlen(ack), 0); // Send valid ACK
        }

        packetNumber++;
    }
    
    close(new_socket);
    close(server_fd);
    return 0;
}
