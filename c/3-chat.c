#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[1024];

    // Create a TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
        error("Socket creation failed");

    // Initialize server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        error("Binding failed");

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0)
        error("Listening failed");

    printf("Server is listening on port %d...\n", PORT);

    // Accept incoming connection
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket < 0)
        error("Acceptance failed");

    printf("Client connected\n");

    // Communication loop
    while (1) {
        // Receive message from client
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received < 0)
            error("Receiving failed");

        if (bytes_received == 0) {
            printf("Client disconnected\n");
            break;
        }

        buffer[bytes_received] = '\0'; // Null-terminate the received message
        printf("Client: %s\n", buffer);

        // Send message to client
        printf("Server: ");
        fgets(buffer, sizeof(buffer), stdin); // Read message from stdin
        send(client_socket, buffer, strlen(buffer), 0);
    }

    // Close sockets
    close(client_socket);
    close(server_socket);

    return 0;
}


// client code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Create a TCP socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
        error("Socket creation failed");

    // Initialize server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0)
        error("Invalid address/ Address not supported");

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        error("Connection failed");

    printf("Connected to server\n");

    // Communication loop
    while (1) {
        // Send message to server
        printf("Client: ");
        fgets(buffer, sizeof(buffer), stdin); // Read message from stdin
        send(client_socket, buffer, strlen(buffer), 0);

        // Receive message from server
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received < 0)
            error("Receiving failed");

        if (bytes_received == 0) {
            printf("Server disconnected\n");
            break;
        }

        buffer[bytes_received] = '\0'; // Null-terminate the received message
        printf("Server: %s\n", buffer);
    }

    // Close socket
    close(client_socket);

    return 0;
}
