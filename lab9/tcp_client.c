//CS22B1093 ROHAN G

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#define SERVER_IP "127.0.0.1" 
#define SERVER_PORT 8080
#define MTU_SIZE 512      


int calculate_total_packets(const char *filename) {
    FILE *file = fopen(filename, "rb");
    fseek(file, 0, SEEK_END);
    int file_size = ftell(file); 
    fclose(file);  

    return (file_size + MTU_SIZE - 1) / MTU_SIZE; 
}

void send_file(int sockfd, FILE *file, int total_packets) {
    char buffer[MTU_SIZE];
    
    int bytes_read, packet_count = 0;

    while ((bytes_read = fread(buffer, sizeof(char), MTU_SIZE, file)) > 0) {
        packet_count++;
        send(sockfd, buffer, bytes_read, 0);
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    FILE *file = fopen("birds-chirping-75156.mp3", "rb");
    if (!file) {
        perror("File open error");
        return 1;
    }
    int total_packets = calculate_total_packets("birds-chirping-75156.mp3");
    printf("Total Packets to Send: %d\n", total_packets);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    send_file(sockfd, file, total_packets);
    printf("File sent successfully.\n");

    fclose(file);
    close(sockfd);
    return 0;
}