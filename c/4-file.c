#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

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

    printf("File server is listening on port %d...\n", PORT);

    while (1) {
        // Accept incoming connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0)
            error("Acceptance failed");

        printf("Client connected\n");

        // Receive filename from client
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received < 0)
            error("Receiving failed");

        buffer[bytes_received] = '\0'; // Null-terminate the received message
        char *filename = buffer;

        // Check if the requested file exists
        if (file_exists(filename)) {
            // Open the file and send its contents to the client
            FILE *file = fopen(filename, "rb");
            if (!file)
                error("File opening failed");

            while (1) {
                int bytes_read = fread(buffer, 1, sizeof(buffer), file);
                if (bytes_read > 0) {
                    send(client_socket, buffer, bytes_read, 0);
                } else if (bytes_read == 0) {
                    break;
                } else {
                    error("File reading failed");
                }
            }

            fclose(file);
            printf("File sent successfully\n");
        } else {
            // Send error message to client
            const char *error_msg = "File not found";
            send(client_socket, error_msg, strlen(error_msg), 0);
            printf("File not found\n");
        }

        // Close client socket
        close(client_socket);
    }

    // Close server socket
    close(server_socket);

    return 0;
}


// client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

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

    // Request filename from user
    printf("Enter filename: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

    // Send filename to server
    send(client_socket, buffer, strlen(buffer), 0);

    // Receive file contents from server
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received < 0)
        error("Receiving failed");

    buffer[bytes_received] = '\0'; // Null-terminate the received message

    // Check if the received data is an error message
    if (strcmp(buffer, "File not found") == 0) {
        printf("Error: File not found\n");
    } else {
        // Display file contents
        printf("File contents:\n%s\n", buffer);
    }

    // Close socket
    close(client_socket);

    return 0;
}
