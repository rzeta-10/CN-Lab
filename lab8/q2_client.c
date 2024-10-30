// CS22B1093 ROHAN G

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 8080
#define BUFFER_SIZE 1024

struct sockaddr_in serv_addr;

char* get_ip_address() {
    static char ip_address[INET_ADDRSTRLEN]; 
    struct hostent *host;
    char hostname[1024];
    gethostname(hostname, sizeof(hostname));
    
    host = gethostbyname(hostname);
    if (host == NULL) {
        printf("Unable to get host information\n");
        return NULL;
    }

    struct in_addr *addr = (struct in_addr *) host->h_addr_list[0];
    inet_ntop(AF_INET, addr, ip_address, sizeof(ip_address));

    return ip_address; 
}

int main() {
    int sock = 0;
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY; 

    char* ip_address = get_ip_address();
    if (ip_address == NULL) {
        return -1; 
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed\n");
        return -1;
    }

    send(sock, ip_address, strlen(ip_address), 0);
    printf("IP Address sent: %s\n", ip_address);

    int valread = read(sock, buffer, BUFFER_SIZE);
    printf("Received from server: %s\n", buffer);

    close(sock);
    return 0;
}