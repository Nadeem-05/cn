#server.py - note... this is two different files...seperate them and run them at the same time. the server code first( this is for keerthi)

import socket
from urllib.parse import urlparse

def download_webpage(url):
    # Parse URL to get host and path
    parsed_url = urlparse(url)
    host = parsed_url.netloc
    path = parsed_url.path if parsed_url.path else '/'

    # Create a TCP socket
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        # Connect to the web server
        s.connect((host, 80))

        # Send HTTP GET request
        request = f"GET {path} HTTP/1.1\r\nHost: {host}\r\nConnection: close\r\n\r\n"
        s.sendall(request.encode())

        # Receive data from the server
        response = b''
        while True:
            data = s.recv(1024)
            if not data:
                break
            response += data

    return response

def save_as_html(response, filename):
    # Extract content from HTTP response
    headers, body = response.split(b'\r\n\r\n', 1)
    content_type = headers.split(b'Content-Type: ')[1].split(b';')[0].decode()
    if content_type == 'text/html':
        # Write content to HTML file
        with open(filename, 'wb') as f:
            f.write(body)

if __name__ == "__main__":
    url = input("Enter the URL of the web page to download: ")
    filename = input("Enter the filename to save the downloaded HTML: ")

    response = download_webpage(url)
    save_as_html(response, filename)

    print(f"Web page downloaded and saved as {filename}")


#client.py

import socket

def echo_client(host, port):
    # Create a TCP socket
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
        # Connect to the server
        client_socket.connect((host, port))

        # Send data to server
        message = input("Enter message to send to server: ")
        client_socket.sendall(message.encode())

        # Receive response from server and echo it
        response = client_socket.recv(1024)
        print("Received:", response.decode())

if __name__ == "__main__":
    HOST = '127.0.0.1'  # The server's hostname or IP address
    PORT = 65432        # The port used by the server
    echo_client(HOST, PORT)
