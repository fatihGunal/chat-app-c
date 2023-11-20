#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 8080
#define BUFFER_SIZE 1024

char* gethtml(const char *filepath) {
    FILE *htmlFile = fopen(filepath, "r");

    if (htmlFile == NULL) {
        perror("Error opening file");
        return NULL;
    }

    fseek(htmlFile, 0, SEEK_END);
    long fileSize = ftell(htmlFile);
    fseek(htmlFile, 0, SEEK_SET);

    char *fullFile = (char *)malloc(fileSize + 1);

    if (fullFile == NULL) {
        perror("Memory allocation error");
        fclose(htmlFile);
        return NULL;
    }

    size_t bytesRead = fread(fullFile, 1, fileSize, htmlFile);
    if (bytesRead != fileSize) {
        perror("Error reading file");
        free(fullFile);
        fclose(htmlFile);
        return NULL;
    }

    fullFile[fileSize] = '\0'; // Null-terminate the string
    fclose(htmlFile);

    char header[] = "HTTP/1.0 200 OK\r\n"
        "Server: webserver-c\r\n"
        "Content-type: text/html\r\n\r\n";

    char* combinedString = (char*)malloc(strlen(header) + strlen(fullFile) + 1);
    strcpy(combinedString, header);
    strcat(combinedString, fullFile);
    free(fullFile);

    return combinedString;
}

int main() {
    char buffer[BUFFER_SIZE];
    const char *filepath = "./index/index.html";
    char *resp = gethtml(filepath);

    printf("%s", resp);
    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("webserver (socket)");
        return 1;
    }
    printf("socket created successfully\n");

    // create the address to bind the socket to
    struct sockaddr_in host_addr;
    int host_addrlen = sizeof(host_addr);

    // Create client address
    struct sockaddr_in client_addr;
    int client_addrlen = sizeof(client_addr);

    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr *)&host_addr, host_addrlen) != 0) {
        perror("webserver (bind)");
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
        // Accept incoming connections
        int newsockfd = accept(sockfd, (struct sockaddr *)&host_addr, (socklen_t *)&host_addrlen);

        if (newsockfd < 0) {
            perror("webserver (accept)");
            continue;
        }
        printf("connection accepted\n");

        // get client address
        int sockn = getsockname(newsockfd, (struct sockaddr *)&client_addr,
                                (socklen_t *)&client_addrlen);
        if (sockn < 0) {
            perror("webserver (getsockname)");
            continue;
        }

        // Read from the socket
        int valread = read(newsockfd, buffer, BUFFER_SIZE);
        if (valread < 0) {
            perror("webserver (read)");
            continue;
        }

        // Read the request
        char method[BUFFER_SIZE], uri[BUFFER_SIZE], version[BUFFER_SIZE];
        sscanf(buffer, "%s %s %s", method, uri, version);
        printf("[%s:%u] methid: %s version: %s uri: %s\n", inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port), method, version, uri);

        // Write to the socket
        int valwrite = write(newsockfd, resp, strlen(resp));
        if (valwrite < 0) {
            perror("webserver (write)");
            continue;
        }

        close(newsockfd);
    }
    return 0;
}

// void gethtml(char *filepath, char* buffer, size_t size) {
//     FILE *htmlFile = fopen(filepath, "r");
//     char fullFile[500] = " ";

//     if (htmlFile == NULL) {
//         perror("Error opening file");
//     }

//     if (htmlFile != NULL) {
//         while (fgets(buffer, (sizeof buffer), htmlFile) != NULL) {
//             strcat(fullFile, buffer);
//         }
//         fclose(htmlFile);
//     }
// }