#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 53
#define BUFFER_SIZE 1024

char *DNS_RECORDS[][2] = {
    {"example.com", "192.168.1.100"},
    {"test.com", "192.168.1.101"},
    {"foo.com", "192.168.1.102"}
};

int NUM_RECORDS = sizeof(DNS_RECORDS) / sizeof(DNS_RECORDS[0]);

char *handle_dns_query(const char *domain) {
    char *response = "Not found";
    for (int i = 0; i < NUM_RECORDS; ++i) {
        if (strcmp(domain, DNS_RECORDS[i][0]) == 0) {
            response = DNS_RECORDS[i][1];
            printf("DNS query for %s: %s\n", domain, response);
            break;
        }
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
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    printf("DNS server is listening on port %d...\n", PORT);

    while (1) {
        // Receive data from client
        int bytes_received = recvfrom(server_socket, buffer, BUFFER_SIZE, 0,
                                      (struct sockaddr *)&client_addr, &client_addr_len);
        if (bytes_received < 0) {
            perror("Receiving failed");
            exit(EXIT_FAILURE);
        }

        buffer[bytes_received] = '\0'; // Null-terminate the received message

        // Handle DNS query and send response to client
        char *response = handle_dns_query(buffer);
        sendto(server_socket, response, strlen(response), 0,
               (const struct sockaddr *)&client_addr, client_addr_len);
    }

    // Close server socket
    close(server_socket);

    return 0;
}


//client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1) {
        // Get the domain name from the user
        printf("Enter domain name (or 'exit' to quit): ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        // Send DNS query to server
        sendto(client_socket, buffer, strlen(buffer), 0,
               (const struct sockaddr *)&server_addr, server_addr_len);

        // Receive response from server
        int bytes_received = recvfrom(client_socket, buffer, BUFFER_SIZE, 0,
                                      (struct sockaddr *)&server_addr, &server_addr_len);
        if (bytes_received < 0) {
            perror("Receiving failed");
            exit(EXIT_FAILURE);
        }

        buffer[bytes_received] = '\0'; // Null-terminate the received message

        // Print the IP address received from the server
        printf("IP address for %s: %s\n", buffer, buffer);
    }

    // Close socket
    close(client_socket);

    return 0;
}
