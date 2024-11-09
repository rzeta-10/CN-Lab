// CS22B1093 ROHAN G

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define HEADER_SIZE 20      
#define NUM_TRIALS 100     

typedef struct {
    int mtu;
    int total_packets;
    int fragmented_packets;
    double transmission_efficiency;
    double packet_loss_rate;
    double average_latency;
    int retransmissions;
} TransmissionStats;

void simulate_transmission(TransmissionStats *stats, int file_size, int mtu, int use_tcp) {
    int data_per_packet = mtu - HEADER_SIZE;
    stats->mtu = mtu;

    stats->total_packets = ceil((double)file_size / data_per_packet);

    stats->fragmented_packets = (file_size > mtu) ? stats->total_packets : 0;

    stats->transmission_efficiency = (double)file_size / (stats->total_packets * mtu);

    if (use_tcp) {
        stats->packet_loss_rate = 0.01;  
        stats->retransmissions = (int)(stats->packet_loss_rate * stats->total_packets);
    } else {
        stats->packet_loss_rate = 0.05;  
        stats->retransmissions = 0;  
    }

    stats->average_latency = stats->total_packets * 0.001 + stats->retransmissions * 0.005;
}

int main() {
    const int mtu_sizes[] = {1500, 512, 256, 128};
    int num_mtu_sizes = sizeof(mtu_sizes) / sizeof(mtu_sizes[0]);

    TransmissionStats tcp_stats[num_mtu_sizes];
    TransmissionStats udp_stats[num_mtu_sizes];

    FILE *audio_file = fopen("birds-chirping-75156.mp3", "rb");
    if (audio_file == NULL) {
        perror("Failed to open audio file");
        return 1;
    }

    fseek(audio_file, 0, SEEK_END);
    int file_size = ftell(audio_file);
    fseek(audio_file, 0, SEEK_SET);
    fclose(audio_file);

    printf("File size: %d bytes\n", file_size);
    printf("Simulating transmission for various MTU sizes...\n\n");

    for (int i = 0; i < num_mtu_sizes; i++) {
        int mtu = mtu_sizes[i];

        simulate_transmission(&tcp_stats[i], file_size, mtu, 1);

        simulate_transmission(&udp_stats[i], file_size, mtu, 0);

        printf("MTU: %d bytes\n", mtu);
        printf("  TCP Transmission:\n");
        printf("    Total Packets: %d\n", tcp_stats[i].total_packets);
        printf("    Fragmented Packets: %d\n", tcp_stats[i].fragmented_packets);
        printf("    Efficiency: %.2f%%\n", tcp_stats[i].transmission_efficiency * 100);
        printf("    Packet Loss Rate: %.2f%%\n", tcp_stats[i].packet_loss_rate * 100);
        printf("    Retransmissions: %d\n", tcp_stats[i].retransmissions);
        printf("    Average Latency: %.2f ms\n", tcp_stats[i].average_latency * 1000);

        printf("  UDP Transmission:\n");
        printf("    Total Packets: %d\n", udp_stats[i].total_packets);
        printf("    Fragmented Packets: %d\n", udp_stats[i].fragmented_packets);
        printf("    Efficiency: %.2f%%\n", udp_stats[i].transmission_efficiency * 100);
        printf("    Packet Loss Rate: %.2f%%\n", udp_stats[i].packet_loss_rate * 100);
        printf("    Retransmissions: %d\n", udp_stats[i].retransmissions);
        printf("    Average Latency: %.2f ms\n\n", udp_stats[i].average_latency * 1000);
    }

    return 0;
}
