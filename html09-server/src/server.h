#define SERVER_H
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

typedef struct {
    int socket_fd;
    char *address;
    int port;
    struct sockaddr_in ip4info;
} serverinfo;

typedef struct {
    int socket_fd;
    char *adress;
    struct sockaddr_in ip4info;
    socklen_t ip4info_len ;
} client;

void show_socketinfo(serverinfo server) {
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(server.ip4info.sin_addr), ip, INET_ADDRSTRLEN);
    printf("\n============================\n");
    printf("   📡 Socket Information\n");
    printf("============================\n");
    printf("🔢 Socket FD : %d\n", server.socket_fd);
    printf("🌐 Address   : %s\n", ip);
    printf("🔌 Port      : %d\n", ntohs(server.ip4info.sin_port));
    printf("============================\n\n");
}


int start_server(serverinfo server);
void stop_server();



void show_status(const char* message, int status_code, int success) {
    printf("\n============================\n");
    if (success) {
        printf(ANSI_COLOR_GREEN "✅ SUCCESS: %s\n" ANSI_COLOR_RESET, message);
    } else {
        printf(ANSI_COLOR_RED "❌ ERROR: %s\n" ANSI_COLOR_RESET, message);
        printf("Errno: %d (%s)\n", errno, strerror(errno));
    }
    printf("Status Code: %d\n", status_code);
    printf("============================\n\n");
}

void show_clientinfo(int client_fd) {
    struct sockaddr_in peer_addr;
    socklen_t addr_len = sizeof(peer_addr);
    char ip[INET_ADDRSTRLEN];

    if (getpeername(client_fd, (struct sockaddr*)&peer_addr, &addr_len) == 0) {
        inet_ntop(AF_INET, &peer_addr.sin_addr, ip, sizeof(ip));
        printf("\n============================\n");
        printf("   🤝 Client Connected\n");
        printf("============================\n");
        printf("🔢 Client FD : %d\n", client_fd);
        printf("🌐 Address   : %s\n", ip);
        printf("🔌 Port      : %d\n", ntohs(peer_addr.sin_port));
        printf("============================\n\n");
    } else {
        printf(ANSI_COLOR_RED "❌ ERROR: Could not get client info (FD: %d)\n" ANSI_COLOR_RESET, client_fd);
        printf("Errno: %d (%s)\n", errno, strerror(errno));
        printf("============================\n\n");
    }
}

