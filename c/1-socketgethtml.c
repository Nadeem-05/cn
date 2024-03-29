#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

char* download_webpage(const char* url) {
    // Parse URL to get host and path
    char host[256], path[1024];
    sscanf(url, "http://%255[^/]%1023s", host, path);

    // Create a TCP socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        error("Socket creation failed");
    }

    // Connect to the web server
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(80);
    if (inet_pton(AF_INET, host, &server_address.sin_addr) <= 0) {
        error("Invalid address/ Address not supported");
    }

    if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        error("Connection failed");
    }

    // Send HTTP GET request
    char request[1024];
    snprintf(request, sizeof(request), "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", path, host);
    if (send(sockfd, request, strlen(request), 0) < 0) {
        error("Send failed");
    }

    // Receive data from the server
    char* response = malloc(BUFFER_SIZE * sizeof(char));
    if (response == NULL) {
        error("Memory allocation failed");
    }
    ssize_t total_bytes_received = 0;
    ssize_t bytes_received;
    while ((bytes_received = recv(sockfd, response + total_bytes_received, BUFFER_SIZE, 0)) > 0) {
        total_bytes_received += bytes_received;
        response = realloc(response, (total_bytes_received + BUFFER_SIZE) * sizeof(char));
        if (response == NULL) {
            error("Memory reallocation failed");
        }
    }
    if (bytes_received < 0) {
        error("Receive failed");
    }

    close(sockfd);
    return response;
}

void save_as_html(const char* response, const char* filename) {
    // Write content to HTML file
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Unable to open file for writing");
        return;
    }

    char* body_start = strstr(response, "\r\n\r\n");
    if (body_start != NULL) {
        fwrite(body_start + 4, strlen(body_start + 4), 1, file);
    }

    fclose(file);
}

int main() {
    char url[1024], filename[256];
    printf("Enter the URL of the web page to download: ");
    fgets(url, sizeof(url), stdin);
    url[strcspn(url, "\n")] = '\0'; // Remove trailing newline
    printf("Enter the filename to save the downloaded HTML: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0'; // Remove trailing newline

    char* response = download_webpage(url);
    save_as_html(response, filename);
    free(response);

    printf("Web page downloaded and saved as %s\n", filename);

    return 0;
}
