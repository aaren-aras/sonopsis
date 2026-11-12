#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include "sonopsis/config.h"
#include "sonopsis/frame.h"

/*
    Implements a UDP network listener (rx) for incoming raw data packets 
    from external sources (i.e., FPGA streaming transducer data over Ethernet)
    and parsing their header data.

    UDP is chosen here for speed and minimal overhead without worrying about
    a couple missing packets/frames.
*/
int main(void) {
    
    // Specify domain (IPv4), type (datagram), protocol (UDP)
    int rx = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
    if (rx < 0) {
        perror("*ERROR: Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define network identity (local bind address)
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET; // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0 
    address.sin_port = htons(PORT); // set Big-Endian byte order 
    
    // Hook network identity to socket (pass as generic pointer type)
    if (bind(rx, (struct sockaddr*)&address, sizeof(address)) < 0) { 
        perror("*ERROR: Socket bind failed");
        close(rx);
        exit(EXIT_FAILURE);
    }
    printf("*SUCCESS: Socket created and bound! Waiting for UDP frame on port %d...\n", PORT);

    // Receive datagrams
    unsigned char buffer[PAYLOAD_SIZE]; 
    struct sockaddr_in sender_info;
    socklen_t sender_info_len = sizeof(sender_info);

    // Pause execution until UDP packet arrives 
    ssize_t received = recvfrom(
        rx, buffer, sizeof(buffer), 0,
        (struct sockaddr*)&sender_info, &sender_info_len
     );
    printf("*SUCCESS: Received %zd bytes!!!\n", received);

    // Interpret as DataFrame header (frame.h)
    struct DataFrame* frame = (struct DataFrame*)buffer;
    printf("Timestamp: %llu ns\n", (unsigned long long)frame->timestamp); // guaranteed >64 bits
    printf("Channels: %d\n", frame->channel_count);
    printf("Samples per line: %d\n", frame->samples_per_line);

    close(rx);
    return 0;
}