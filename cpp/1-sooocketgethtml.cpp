#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

std::string download_webpage(const std::string& url) {
    // Parse URL to get host and path
    std::string host, path;
    size_t host_end = url.find("/", 8); // find the end of host part
    if (host_end != std::string::npos) {
        host = url.substr(7, host_end - 7);
        path = url.substr(host_end);
    } else {
        host = url.substr(7);
        path = "/";
    }

    // Create a TCP socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Connect to the web server
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(80);
    if (inet_pton(AF_INET, host.c_str(), &server_address.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Send HTTP GET request
    std::string request = "GET " + path + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";
    if (send(sockfd, request.c_str(), request.length(), 0) < 0) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }

    // Receive data from the server
    std::string response;
    char buffer[1024] = {0};
    int bytes_received;
    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
        response.append(buffer, bytes_received);
    }
    if (bytes_received < 0) {
        perror("Receive failed");
        exit(EXIT_FAILURE);
    }

    close(sockfd);
    return response;
}

void save_as_html(const std::string& response, const std::string& filename) {
    // Write content to HTML file
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        size_t body_start = response.find("\r\n\r\n") + 4;
        if (body_start != std::string::npos) {
            file.write(response.c_str() + body_start, response.length() - body_start);
        }
        file.close();
    } else {
        std::cerr << "Unable to open file for writing" << std::endl;
    }
}

int main() {
    std::string url, filename;
    std::cout << "Enter the URL of the web page to download: ";
    std::getline(std::cin, url);
    std::cout << "Enter the filename to save the downloaded HTML: ";
    std::getline(std::cin, filename);

    std::string response = download_webpage(url);
    save_as_html(response, filename);

    std::cout << "Web page downloaded and saved as " << filename << std::endl;

    return 0;
}
