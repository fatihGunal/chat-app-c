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
    struct sockaddr_in* server_addr = malloc(sizeof(struct sockaddr_in));

    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(port);
    server_addr->sin_addr.s_addr = htonl(INADDR_ANY);

    printf("Server address created successfully\n");

    return server_addr;
}

void receiveClientAndPrint(int serverSocketFD) {
    // Create client address
    struct sockaddr_in client_addr;
    int client_addrlen = sizeof(client_addr);

    for (;;) {
        int newsockfd = accept(serverSocketFD, (struct sockaddr*)&client_addr, (socklen_t *)&client_addrlen);

        if (newsockfd < 0) {
            perror("webserver (accept)");
            continue;
        }
    }

    shutdown(serverSocketFD, SHUT_RDWR);
}
int main() {
    printf("Hello chat server!\n");

    int sockfd = createSocket();

    // create address to bind socket
    // struct sockaddr_in *server_addr = createServerAddress(PORT);
    struct sockaddr_in* server_addr = malloc(sizeof(struct sockaddr_in));

    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(port);
    server_addr->sin_addr.s_addr = htonl(INADDR_ANY);

    printf("Server address created successfully\n");
    int server_addrlen = sizeof(server_addr);

    if (bind(sockfd, (struct sockaddr *)&server_addr, server_addrlen) != 0) {
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

    receiveClientAndPrint(sockfd);
    return 0;
}
