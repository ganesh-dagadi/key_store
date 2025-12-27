#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>


static void die(const char *msg) {
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        die("socket()");
    }

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(8080);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);  // 127.0.0.1
    int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv) {
        die("connect");
    }

    uint8_t msg[24];
    msg[0] = 0; // version
    msg[1] = 0; // opcode
    msg[2] = 32; // flag
    msg[3] = 1; // flag
    msg[4] = 0; //payloadlen
    msg[5] = 0; //payload len
    msg[6] = 0; //payload len
    msg[7] = 16; //payload len
    msg[8] = 0; // key len byte 1
    msg[9] = 3; // key len byte 2
    msg[10] = 'd'; //key
    msg[11] = 'i'; //key
    msg[12] = 'c'; //key
    msg[13] = 0; // value_len byte 1
    msg[14] = 0; // value_len byte 2
    msg[15] = 0; // value_len byte 3
    msg[16] = 3; // value_len byte 4
    msg[17] = 'b'; // value
    msg[18] = 'o'; // value
    msg[19] = 'b'; // value
    msg[20] = 0; //ttl
    msg[21] = 0;//ttl
    msg[22] = 0; //ttl
    msg[23] = 50; //ttl
    write(fd, msg, 24);

    close(fd);
    return 0;
}