//CS22B1093 ROHAN G

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define SERVER_PORT 8080
#define MTU_SIZE 512
#define PACKET_LOSS_PROB 0.05 
#define MAX_LATENCY 100       
#define DISPLAY_PACKET_COUNT 20  

typedef struct {
    int packet_id;
    int latency;
} PacketLatency;

void receive_file(int sockfd) {
    FILE *file = fopen("received_audio_udp.mp3", "wb");
    if (!file) {
        perror("File open error");
        close(sockfd);
        return;
    }

    char buffer[MTU_SIZE];
    int bytes_received;
    int packets_received = 0, packets_lost = 0;
    int total_latency = 0;
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(client_addr);
    
    PacketLatency latencies[1000]; 
    int latency_index = 0;

    srand(time(NULL));

    struct timeval timeout;
    timeout.tv_sec = 5; 
    timeout.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("Error setting socket timeout");
        fclose(file);
        close(sockfd);
        return;
    }

    while ((bytes_received = recvfrom(sockfd, buffer, MTU_SIZE, 0, (struct sockaddr*)&client_addr, &addr_size)) > 0) {
        float loss_chance = (float)rand() / RAND_MAX;
         packets_received++;
        if (loss_chance < PACKET_LOSS_PROB) {
            packets_lost++;
            continue; 
        }
        int latency = rand() % MAX_LATENCY + 1;
        total_latency += latency;

        fwrite(buffer, sizeof(char), bytes_received, file);
    }

    float average_latency = packets_received > 0 ? (float)total_latency / packets_received : 0;

    printf("Packets Received: %d\n", packets_received-packets_lost);
    printf("Packets Lost: %d\n", packets_lost);
    printf("Average Latency per Packet: %.2f ms\n", average_latency);
    printf("Total Transmission Latency: %d ms\n", total_latency);
    printf("Efficiency : %.2f%%\n",(float)(packets_received-packets_lost)/packets_received*100);
    printf("Packet Loss Rate: %.2f%%\n", (float)packets_lost / packets_received * 100);
    printf("Reassemble reliability: %.2f%%\n", (float)(packets_received-packets_lost)/packets_received*100);

    fclose(file);
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    receive_file(sockfd);

    close(sockfd);
    return 0;
}