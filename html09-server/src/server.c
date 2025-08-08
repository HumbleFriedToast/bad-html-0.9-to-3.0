#include "server.h"
#include <fcntl.h>
#include <stdbool.h>






int start_server(serverinfo server) {
    int temp_sock =  socket(PF_INET,SOCK_STREAM,0);
    int error = bind(temp_sock, (struct sockaddr*)&server.ip4info, sizeof(server.ip4info));
    show_status("Binding process",error,error+1);
    return temp_sock;
};

bool verify_message(char *message){
    if (message == NULL || strlen(message) == 0) {
        return false; // Invalid message
    }
    if (strlen(message) > 1000) { 
        return false; 
    }
    return true; 
};

int parse_message(char *message){
    char buffer[3] = {0};
    strncpy(buffer, message, 3);
    if (strcmp(buffer, "GET") == 0) {
        return 1; // GET request
    } else if (strcmp(buffer, "POST") == 0) {
        return 2; // POST request  
    return 0;
};
}

char* process_message(int message_code,char* message,char* response){
    char buffer[256] = {0}; 
    switch (message_code) {
        case 1:
            printf("Handling GET request: %s\n", message);
            strcpy(buffer,message+3);
            //printf("Data: %s\n", buffer); //debugging purposes
            FILE *fp = fopen(buffer+2,"r");
            if (fp!= NULL){
                //printf("File %s opened successfully.\n", buffer+2);
                char file_buffer[1024];
                while (fgets(file_buffer, sizeof(file_buffer), fp) != NULL) {
                    printf("%s", file_buffer); 
                    strcat(response, file_buffer); //append the file content to response
                }
                fclose(fp);
                printf("File %s closed successfully.\n", buffer+2);
            } else {
                printf("Error opening file %s.\n", buffer+2);
                strcat(response, "HTTP/0.9 404 Not Found\nFile not found.\n");
                
            } 
        

            break;
        case 2:
            printf("Handling POST request: %s\n", message);
            // what does post do?do i save file into the server? (designing post commands later)
            break;
        default:
            printf("Unknown request type.\n");
    }
}

int clean_message(char *message){
    for(int i = 0; message[i] != '\0'; i++) {
        if (message[i] == '\\' || message[i] == ' ') {
           return 1;        }
    }
    return 0;
};

int main() {
    char buffer[10000];
    char message[1000];
    size_t buffer_len = sizeof(buffer);
    size_t message_len = sizeof(message);
    //prereq data
    client client_list[10];
    // Initialize the server
    serverinfo server = {0
        ,"192.168.1.6"
        ,1885,
        {AF_INET,htons(1885),0}
    };
    inet_pton(AF_INET, "192.168.1.6", &server.ip4info.sin_addr);
    int socket = start_server(server);
    //debug and logs
    show_socketinfo(server);


    listen(socket,10);
    for(int i = 0;i<=10;i++){
    client_list[i].ip4info_len = sizeof(client_list[i].ip4info);
    client_list[i].socket_fd = accept(socket, (struct sockaddr*)&client_list[i].ip4info,&client_list[i].ip4info_len);
    show_clientinfo(client_list[i].socket_fd);
    recv(client_list[i].socket_fd,buffer,21,0);
    process_message(1,buffer,message);
    memset(buffer, 0, sizeof(buffer));
    send(client_list[i].socket_fd,message,message_len,0);
    memset(message, 0, sizeof(message));
    
    
    }

    return 0;
}
