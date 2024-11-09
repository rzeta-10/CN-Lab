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

void receive_file(int client_sock, int total_packets) {
    FILE *file = fopen("received_audio.mp3", "wb");
    char buffer[MTU_SIZE];
    int bytes_received;
    int packets_received = 0, packets_lost = 0, retransmissions = 0;
    int total_latency = 0;

    PacketLatency latencies[total_packets];
    int latency_index = 0;

    srand(time(NULL));

    while ((bytes_received = recv(client_sock, buffer, MTU_SIZE, 0)) > 0) {
        packets_received++;
        float loss_chance = (float)rand() / RAND_MAX;

        if (loss_chance < PACKET_LOSS_PROB) {
            packets_lost++;
            retransmissions++;
            continue; 
        }

        int latency = rand() % MAX_LATENCY + 1;
        total_latency += latency;
        //usleep(latency * 1000); 
        fwrite(buffer, sizeof(char), bytes_received, file);
    }

    float average_latency = (float)total_latency / packets_received;

    printf("Packets Received: %d\n", packets_received);
    printf("Packets Lost: %d\n", packets_lost);
    printf("Retransmissions: %d\n", retransmissions);
    printf("Average Latency per Packet: %.2f ms\n", average_latency);
    printf("Total Transmission Latency: %d ms\n", total_latency);
    printf("Efficiency : %.2f%%\n",(float)(packets_received-packets_lost)/packets_received*100);
    printf("Packet Loss Rate: %.2f%%\n", (float)packets_lost / packets_received * 100);
    printf("Reassemble reliability: %.2f%%\n", (float)(packets_received-packets_lost)/packets_received*100);

    fclose(file);
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_sock, 1);

    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size);

    int total_packets = 0;
    receive_file(client_sock, total_packets);
    close(client_sock);
    close(server_sock);
    return 0;
}