#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


void gethtml(char *filepath, char* buffer, size_t size) {
    printf("%s -> %s -> %zu", filepath, buffer, size);
    FILE *htmlFile = fopen(filepath, "r");
    // char buffer[100];
    char fullFile[500] = " ";

    if (htmlFile == NULL) {
        perror("Error opening file");
    }

    if (htmlFile != NULL) {
        while (fgets(buffer, (sizeof buffer), htmlFile) != NULL) {
            // printf("%s", buffer);
            strcat(fullFile, buffer);
        }
        fclose(htmlFile);
        
    }
    printf("%s", fullFile);
}

int main() {
    char filePath[30] = "./index/index.html";
    char buffer[20];

    gethtml(filePath, buffer, sizeof(buffer));
    printf("test");
    return 0;
}

