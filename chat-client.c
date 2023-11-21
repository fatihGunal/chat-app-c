#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define _GNU_SOURCE
#define PORT 8080
#define BUFFER_SIZE 1024


void readConsoleAndSend(int socketfd) {
    char *name = NULL;
    size_t nameSize = 0;
    printf("Please enter your name..\n");

    ssize_t nameCount = getline(&name, &nameSize, stdin);
    name[nameCount-1]=0;

    char *line = NULL;
    size_t lineSize = 0;
    printf("Type message...\n");

    char buffer[BUFFER_SIZE];

    while(1)
    {
        if (getline(&line, &lineSize, stdin) == -1)
        {
            perror("client (getline)");
        }
        if (strcmp(line, "exit") == 0) {
            printf("Exiting...");
            break;
        }
        sprintf(buffer, "%s: %s", name, line);
        send(socketfd, buffer, strlen(buffer), 0);

        ssize_t amountRec = recv(socketfd, buffer, BUFFER_SIZE, 0);
        if (amountRec > 0) {
            printf("reply: %s", buffer);
        }
    }
}


int main() {
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    char *ip = "127.0.0.1";
    if (socketfd == -1) {
        perror("webserver (socket)");
        return 1;
    }

    struct sockaddr_in address;
    int addresslen = sizeof(address);

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    inet_pton(AF_INET, ip, &address.sin_addr.s_addr);
    int result = connect(socketfd, (struct sockaddr *)&address, addresslen);

    if (result == 0) {
        printf("connection was succesfully!\n");
    }

    readConsoleAndSend(socketfd);
    close(socketfd);
}