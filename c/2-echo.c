#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void echo_server(const char* host, int port) {
    // Create a TCP socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        return;
    }

    // Bind the socket to the address and port
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(host);
    server_address.sin_port = htons(port);
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Binding failed");
        close(server_socket);
        return;
    }

    // Listen for incoming connections
    listen(server_socket, 5);
    printf("Echo server is listening on %s:%d\n", host, port);

    // Accept incoming connection
    int client_socket;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len)) < 0) {
        perror("Acceptance failed");
        close(server_socket);
        return;
    }
    printf("Connection established with %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

    // Receive data from client and echo it back
    char buffer[1024];
    int bytes_received;
    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        send(client_socket, buffer, bytes_received, 0);
    }
    if (bytes_received < 0) {
        perror("Receiving failed");
    }

    close(client_socket);
    close(server_socket);
}

int main() {
    const char* HOST = "127.0.0.1";
    const int PORT = 12345;

    printf("Starting echo server...\n");
    echo_server(HOST, PORT);

    return 0;
}


// Client code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void echo_client(const char* host, int port) {
    // Create a TCP socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        return;
    }

    // Connect to the server
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(host);
    server_address.sin_port = htons(port);
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        close(client_socket);
        return;
    }

    // Send data to server
    printf("Enter message to send to server: ");
    char message[1024];
    fgets(message, sizeof(message), stdin);
    send(client_socket, message, strlen(message), 0);

    // Receive response from server and echo it
    char buffer[1024];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received > 0) {
        printf("Received: %s\n", buffer);
    } else if (bytes_received == 0) {
        printf("Server closed the connection\n");
    } else {
        perror("Receiving failed");
    }

    close(client_socket);
}

int main() {
    const char* HOST = "127.0.0.1";
    const int PORT = 12345;

    printf("Starting echo client...\n");
    echo_client(HOST, PORT);

    return 0;
}
