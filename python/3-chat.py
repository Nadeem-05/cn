import socket

def main():
    HOST = '127.0.0.1'  # Standard loopback interface address (localhost)
    PORT = 12345        # Port to listen on (non-privileged ports are > 1023)

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.bind((HOST, PORT))
        server_socket.listen()

        print(f"Server is listening on {HOST}:{PORT}")

        conn, addr = server_socket.accept()
        print('Connected by', addr)

        with conn:
            while True:
                data = conn.recv(1024)
                if not data:
                    break
                print(f"Client: {data.decode()}")
                message = input("Server: ")
                conn.sendall(message.encode())

if __name__ == "__main__":
    main()

# client code

import socket

def main():
    HOST = '127.0.0.1'  # The server's hostname or IP address
    PORT = 12345        # The port used by the server

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
        client_socket.connect((HOST, PORT))

        print(f"Connected to {HOST}:{PORT}")

        while True:
            message = input("Client: ")
            client_socket.sendall(message.encode())
            data = client_socket.recv(1024)
            print(f"Server: {data.decode()}")

if __name__ == "__main__":
    main()
