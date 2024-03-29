import socket
import os

def error(msg):
    print(msg)
    exit(1)

def file_exists(filename):
    return os.path.exists(filename)

def main():
    HOST = '127.0.0.1'
    PORT = 12345

    # Create a TCP socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((HOST, PORT))
    server_socket.listen()

    print(f"File server is listening on {HOST}:{PORT}")

    while True:
        # Accept incoming connection
        client_socket, client_addr = server_socket.accept()
        print(f"Client connected: {client_addr}")

        # Receive filename from client
        filename = client_socket.recv(1024).decode()
        print(f"Requested file: {filename}")

        # Check if file exists
        if file_exists(filename):
            with open(filename, 'rb') as file:
                file_data = file.read()

            # Send file contents to client
            client_socket.sendall(file_data)
            print("File sent successfully")
        else:
            # Send error message to client
            error_msg = "File not found"
            client_socket.sendall(error_msg.encode())
            print("File not found")

        # Close client socket
        client_socket.close()

if __name__ == "__main__":
    main()


# client

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
