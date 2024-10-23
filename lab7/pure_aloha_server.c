// CS22B1093 ROHAN G

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8081
#define MAX_BUFFER 1024
#define PACKET_LOSS_PROBABILITY 30 

int simulatePacketLossOrCorruption() {
    return (rand() % 100 < PACKET_LOSS_PROBABILITY);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX_BUFFER] = {0};
    char ack[] = "ACK";             
    char corruptedAck[] = "CORRUPTED_ACK"; 
    
    srand(time(0)); 

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Server is listening on port %d...\n", PORT);

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    int packetNumber = 1;
    while (1) {
        memset(buffer, 0, MAX_BUFFER);
        int valread = read(new_socket, buffer, MAX_BUFFER); 
        
        if (valread == 0) {
            printf("Connection closed by client.\n");
            break;
        }

        printf("\nReceived packet %d: %s\n", packetNumber, buffer);

        if (simulatePacketLossOrCorruption()) {
            if (rand() % 2 == 0) {
                printf("Packet %d lost. No ACK sent.\n", packetNumber);
            } else {
                printf("Packet %d corrupted. Sending corrupted ACK...\n", packetNumber);
                send(new_socket, corruptedAck, strlen(corruptedAck), 0);
            }
        } else {
            printf("Packet %d received successfully. Sending ACK...\n", packetNumber);
            send(new_socket, ack, strlen(ack), 0);
        }

        packetNumber++;
    }

    close(new_socket);
    close(server_fd);
    return 0;
}