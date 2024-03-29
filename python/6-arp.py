import socket
import struct
import binascii

def send_arp_request(iface, target_ip):
    # Create a raw socket
    sock = socket.socket(socket.AF_PACKET, socket.SOCK_RAW)

    # Get source MAC address
    src_mac = hex(get_mac_address(iface)).lstrip('0x')
    src_mac = src_mac[:2] + ':' + src_mac[2:4] + ':' + src_mac[4:6] + ':' + src_mac[6:8] + ':' + src_mac[8:10] + ':' + src_mac[10:12]

    # Ethernet header
    ether_header = struct.pack('!6s6s2s', binascii.unhexlify('ff'*6), binascii.unhexlify(src_mac.replace(':', '')), b'\x08\x06')

    # ARP header
    arp_header = struct.pack('!HHBBH6s4s6s4s', 1, 0x0800, 6, 4, 1, binascii.unhexlify(src_mac.replace(':', '')),
                             socket.inet_aton('0.0.0.0'), binascii.unhexlify('00'*6), socket.inet_aton(target_ip))

    # Send ARP request
    sock.send(ether_header + arp_header)

def send_rarp_request(iface, target_mac):
    # RARP is obsolete and not widely supported, so we won't implement it here
    print("RARP is obsolete and not widely supported.")

def get_mac_address(iface):
    # Get the MAC address of the given interface
    try:
        with open(f'/sys/class/net/{iface}/address') as f:
            mac = f.read().strip()
            return int(mac.replace(':', ''), 16)
    except IOError:
        print("Error: Interface not found or unable to read MAC address.")
        exit(1)

def main():
    iface = input("Enter the interface name: ")
    target_ip = input("Enter the target IP address for ARP request: ")
    send_arp_request(iface, target_ip)

    target_mac = input("Enter the target MAC address for RARP request: ")
    send_rarp_request(iface, target_mac)

if __name__ == "__main__":
    main()
