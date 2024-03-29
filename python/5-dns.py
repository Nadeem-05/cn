import socket

# DNS records mapping domain names to IP addresses
DNS_RECORDS = {
    "example.com": "192.168.1.100",
    "test.com": "192.168.1.101",
    "foo.com": "192.168.1.102"
}

def handle_dns_query(data):
    # Extract the domain name from the DNS query
    domain = data.decode().strip()

    # Look up the IP address for the domain in the DNS records
    if domain in DNS_RECORDS:
        ip_address = DNS_RECORDS[domain]
        print(f"DNS query for {domain}: {ip_address}")
        return ip_address.encode()
    else:
        print(f"DNS query for {domain}: Not found")
        return b"Not found"

def main():
    # Create a UDP socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Bind the socket to localhost and port 53 (standard DNS port)
    server_socket.bind(("127.0.0.1", 53))

    print("DNS server is listening on port 53...")

    while True:
        # Receive data from client
        data, client_address = server_socket.recvfrom(1024)
        print(f"Received DNS query from {client_address}")

        # Handle DNS query and send response to client
        response = handle_dns_query(data)
        server_socket.sendto(response, client_address)

if __name__ == "__main__":
    main()


# client 

import socket

def main():
    # Create a UDP socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # DNS server address and port
    server_address = ("127.0.0.1", 53)

    while True:
        # Get the domain name from the user
        domain = input("Enter domain name (or 'exit' to quit): ")

        if domain.lower() == "exit":
            break

        # Send DNS query to server
        client_socket.sendto(domain.encode(), server_address)

        # Receive response from server
        data, _ = client_socket.recvfrom(1024)
        response = data.decode()

        # Print the IP address received from the server
        print(f"IP address for {domain}: {response}")

    # Close the socket
    client_socket.close()

if __name__ == "__main__":
    main()
