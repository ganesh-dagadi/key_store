#pragma once
#include <string>
#include <arpa/inet.h>

// Server configurations
#define SERVER_IP_ADDRESS   inet_addr("127.0.0.1")
#define SERVER_PORT_NUMBER  htons(9999)
#define domain              AF_INET
#define type                SOCK_STREAM

class Client {
private:
    int socket_fd = -1;
    int connection_fd = -1;
public:
    void connectToServer();
    void disconnectFromServer();
    int32_t readFull(uint8_t *buffer, size_t len) const;
    int32_t writeFull(const uint8_t *buffer, size_t len) const;
    int getSocketFileDescriptor() const;
    int connectionFileDescriptor() const;
};

