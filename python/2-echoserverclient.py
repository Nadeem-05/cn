import socket

def echo_server(host, port):
    # Create a TCP socket
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        # Bind the socket to the address and port
        server_socket.bind((host, port))
        # Listen for incoming connections
        server_socket.listen()

        print(f"Echo server is listening on {host}:{port}")

        # Accept incoming connection
        client_socket, client_address = server_socket.accept()
        with client_socket:
            print(f"Connection established with {client_address}")

            # Receive data from client and echo it back
            while True:
                data = client_socket.recv(1024)
                if not data:
                    break
                client_socket.sendall(data)

if __name__ == "__main__":
    HOST = '127.0.0.1'  # Standard loopback interface address (localhost)
    PORT = 65432        # Port to listen on (non-privileged ports are > 1023)
    echo_server(HOST, PORT)


#client.py

import socket

def error(msg):
    print(msg)
    exit(1)

def main():
    HOST = '127.0.0.1'
    PORT = 12345

    # Create a TCP socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        # Connect to the server
        client_socket.connect((HOST, PORT))
        print(f"Connected to server")

        # Request filename from user
        filename = input("Enter filename: ")
        client_socket.sendall(filename.encode())

        # Receive file contents from server
        file_data = client_socket.recv(1024)

        # Check if the received data is an error message
        if file_data.decode() == "File not found":
            print("Error: File not found")
        else:
            # Display file contents
            print("File contents:")
            print(file_data.decode())

    except Exception as e:
        error(f"Error: {e}")

    finally:
        # Close socket
        client_socket.close()

if __name__ == "__main__":
    main()
