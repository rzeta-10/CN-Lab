//CS22B1093 ROHAN G

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAX 1024

int main() {
    int sockfd;
    char buffer[MAX];
    const char *message = "Hello from ROHAN";
    struct sockaddr_in serv_addr;
    socklen_t len;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    if (inet_pton(AF_INET, "172.16.25.82", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address or Address not supported");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    sendto(sockfd, (const char *)message, strlen(message), 0, (const struct sockaddr *)&serv_addr, sizeof(serv_addr));

    len = sizeof(serv_addr);
    int n = recvfrom(sockfd, (char *)buffer, MAX, MSG_WAITALL, (struct sockaddr *)&serv_addr, &len);
    buffer[n] = '\0'; 
    printf("Server : %s\n", buffer);

    close(sockfd);
    return 0;
}