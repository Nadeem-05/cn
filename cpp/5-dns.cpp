#include <iostream>
#include <map>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 53
#define BUFFER_SIZE 1024

std::map<std::string, std::string> DNS_RECORDS = {
    {"example.com", "192.168.1.100"},
    {"test.com", "192.168.1.101"},
    {"foo.com", "192.168.1.102"}
};

std::string handle_dns_query(const std::string& domain) {
    std::string response;
    if (DNS_RECORDS.find(domain) != DNS_RECORDS.end()) {
        response = DNS_RECORDS[domain];
        std::cout << "DNS query for " << domain << ": " << response << std::endl;
    } else {
        response = "Not found";
        std::cout << "DNS query for " << domain << ": Not found" << std::endl;
    }
    return response;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Create a UDP socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    // Initialize server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Binding failed" << std::endl;
        return 1;
    }

    std::cout << "DNS server is listening on port " << PORT << "..." << std::endl;

    while (true) {
        // Receive data from client
        int bytes_received = recvfrom(server_socket, buffer, BUFFER_SIZE, 0,
                                      (struct sockaddr *)&client_addr, &client_addr_len);
        if (bytes_received < 0) {
            std::cerr << "Receiving failed" << std::endl;
            return 1;
        }

        buffer[bytes_received] = '\0'; // Null-terminate the received message
        std::string domain(buffer);

        // Handle DNS query and send response to client
        std::string response = handle_dns_query(domain);
        sendto(server_socket, response.c_str(), response.length(), 0,
               (const struct sockaddr *)&client_addr, client_addr_len);
    }

    // Close server socket
    close(server_socket);

    return 0;
}


//client 

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 53
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    socklen_t server_addr_len = sizeof(server_addr);
    char buffer[BUFFER_SIZE];

    // Create a UDP socket
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    // Initialize server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (true) {
        // Get the domain name from the user
        std::cout << "Enter domain name (or 'exit' to quit): ";
        std::string domain;
        std::getline(std::cin, domain);

        if (domain == "exit") {
            break;
        }

        // Send DNS query to server
        sendto(client_socket, domain.c_str(), domain.length(), 0,
               (const struct sockaddr *)&server_addr, server_addr_len);

        // Receive response from server
        int bytes_received = recvfrom(client_socket, buffer, BUFFER_SIZE, 0,
                                      (struct sockaddr *)&server_addr, &server_addr_len);
        if (bytes_received < 0) {
            std::cerr << "Receiving failed" << std::endl;
            return 1;
        }

        buffer[bytes_received] = '\0'; // Null-terminate the received message
        std::string response(buffer);

        // Print the IP address received from the server
        std::cout << "IP address for " << domain << ": " << response << std::endl;
    }

    // Close socket
    close(client_socket);

    return 0;
}
