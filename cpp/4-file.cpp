#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345
#define BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

bool file_exists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
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

    std::cout << "File server is listening on port " << PORT << "...\n";

    // Accept incoming connection
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket < 0)
        error("Acceptance failed");

    std::cout << "Client connected\n";

    // Receive filename from client
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received < 0)
        error("Receiving failed");

    buffer[bytes_received] = '\0'; // Null-terminate the received message
    std::string filename(buffer);

    // Check if the requested file exists
    if (file_exists(filename)) {
        std::ifstream file(filename, std::ios::binary);
        std::ostringstream file_content;
        file_content << file.rdbuf();
        std::string file_data = file_content.str();

        // Send file contents to client
        send(client_socket, file_data.c_str(), file_data.length(), 0);
        std::cout << "File sent successfully\n";
    } else {
        // Send error message to client
        const char* error_msg = "File not found";
        send(client_socket, error_msg, strlen(error_msg), 0);
        std::cout << "File not found\n";
    }

    // Close sockets
    close(client_socket);
    close(server_socket);

    return 0;
}


//client 

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

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

    std::cout << "Connected to server\n";

    // Send filename to server
    std::string filename;
    std::cout << "Enter filename: ";
    std::getline(std::cin, filename);
    send(client_socket, filename.c_str(), filename.length(), 0);

    // Receive file contents from server
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received < 0)
        error("Receiving failed");

    if (bytes_received == 0) {
        std::cout << "Server disconnected\n";
    } else {
        buffer[bytes_received] = '\0'; // Null-terminate the received message

        // Check if the received data is an error message
        std::string file_data(buffer);
        if (file_data == "File not found") {
            std::cout << "Error: File not found\n";
        } else {
            // Display file contents
            std::cout << "File contents:\n" << file_data << std::endl;
        }
    }

    // Close socket
    close(client_socket);

    return 0;
}
