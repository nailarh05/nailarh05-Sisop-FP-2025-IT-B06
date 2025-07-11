#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    printf("Server menunggu koneksi di port %d...\n", PORT);

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
    perror("accept error");
    exit(EXIT_FAILURE);
} else {
    printf("ACC bosku! Koneksi aman nih ye... (Socket fd: %d)\n", new_socket);  
}

  while(1) {
        
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread <= 0) break;
        
        printf("Pesan diterima dari client: %s\n", buffer);
        
        char *response;
       
        if (strcmp(buffer, "dragon") == 0) {
            response = "rawr";
        } else if (strcmp(buffer, "wolf") == 0) {
            response = "awoo";
        } else if (strcmp(buffer, "dog") == 0) {
            response = "woof";
        } else {
            response = "unknown :(";
        }
       
        send(new_socket, response, strlen(response), 0);
        printf("Mengirim respons: %s\n", response);
        
        memset(buffer, 0, BUFFER_SIZE);
    }
    
    close(new_socket);
    close(server_fd);
    
    return 0;
}
