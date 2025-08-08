#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    char buffer[1000];
    size_t buffer_len = sizeof(buffer);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1885);
    inet_pton(AF_INET, "192.168.1.6", &server_addr.sin_addr);
    
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sock);
        return 1;
    }
    printf("Connected to server!\n");

    //get specific file
    send(sock, "GET /test/index.html", 21, 0);
    recv(sock, buffer, buffer_len - 1, 0);
    printf(buffer);

    close(sock);
}
