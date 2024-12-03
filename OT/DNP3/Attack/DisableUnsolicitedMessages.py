from scapy.all import *

# DNP3 packet structure
dnp3_packet = DNP3(
    source=0x01,  # Master DNP3 address
    destination=0x02,  # Slave DNP3 address
    function_code=21  # Disable unsolicited messages
)

# IP and TCP packet encapsulation
ip_packet = IP(src="192.168.1.100", dst="192.168.1.2") / TCP(sport=20000, dport=20000) / dnp3_packet

# Send packet
send(ip_packet)
