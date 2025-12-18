#pragma once
#include <cstdint>
#include <unordered_map>
#include <string>

#define IPV4_MAX_STR_LEN 16
class Connection{
    private:
    std::string client_addr;
    std::uint16_t client_port;
    int connection_fd;
    public:
    Connection (std::string client_addr, std::uint16_t port) {
        this->client_addr = client_addr;
        this->client_port = port;
    }

    std::string getAddress();
    int getPort();
};

class Server {
    private:
    int socket_fd;
    int conn_fd;
    std::string bound_ip;
    uint16_t bound_port;
    std::unordered_map<int, Connection> connection_map;

    public:
    int initializeServer();
    int shutdownServer();
    int beginServerListening();
    int stopServer();
};