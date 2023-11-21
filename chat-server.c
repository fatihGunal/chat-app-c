#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>


#define PORT 8080

int createSocket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        perror("chatserver (socket creation failed)");
        return 1;
    }
    printf("socket created successfully\n");

    return sockfd;
}
struct sockaddr_in *createServerAddress(int port) {
    struct sockaddr_in *host_addr = malloc(sizeof(struct host_addr));

    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(port);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    printf("Server address created successfully\n");

    return host_addr;
    // printf("Address Family: %d\n", host_addr.sin_family);
    // printf("Port: %u\n", ntohs(host_addr.sin_port)); 
    // printf("IP Address: %s\n", inet_ntoa(host_addr.sin_addr));
}
int main() {
    printf("Hello chat server!\n");

    int sockfd = createSocket();
    // create address to bind socket
    struct sockaddr_in *host_addr = createServerAddress(PORT);
    int hostaddrlen = sizeof(host_addr);

    // Create client address
    struct sockaddr_in client_addr;
    int client_addrlen = sizeof(client_addr);

    if (bind(sockfd, (struct sockaddr *)&host_addr, hostaddrlen) != 0) {
        perror("webserver bind");
        return 1;
    }
    printf("socket successfully bound to address\n");

    // listen for incoming connections
    if (listen(sockfd, SOMAXCONN) != 0) {
        perror("webserver (listen)");
        return 1;
    }
    printf("server listening for connections\n");

    for (;;) {
        int newsockfd = accept(sockfd, (struct sockaddr*)&host_addr, (socklen_t *)&hostaddrlen);

        if (newsockfd < 0) {
            perror("webserver (accept)");
            continue;
        }
        printf("Connection accepted!");

        int sockn = getsockname(newsockfd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addrlen);

        printf("Address Family: %d\n", client_addr.sin_family);
        printf("Port: %u\n", ntohs(client_addr.sin_port));
        printf("IP Address: %s\n", inet_ntoa(client_addr.sin_addr)); 
        printf("%d\n", sockn);
        close(newsockfd);
    }
    return 0;
}
