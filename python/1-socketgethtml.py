
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

