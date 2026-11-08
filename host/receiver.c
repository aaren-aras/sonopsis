#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include "sonopsis/frame.h"
#include "sonopsis/pipeline_config.h"

#define PORT 5005

int main(void) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(sock, (struct sockaddr*)&addr, sizeof(addr));

    printf("Waiting for UDP frame on port %d...\n", PORT);

    // receive raw bytes
    unsigned char buffer[65536];
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);

    ssize_t received = recvfrom(sock, buffer, sizeof(buffer), 0,
        (struct sockaddr*)&sender, &sender_len);

    printf("Received %zd bytes\n", received);

    // interpret as DataFrame header
    struct DataFrame* frame = (struct DataFrame*)buffer;
    printf("Timestamp:        %llu ns\n", (unsigned long long)frame->timestamp);
    printf("Channels:         %d\n", frame->channel_count);
    printf("Samples per line: %d\n", frame->samples_per_line);

    return 0;
}