#pragma once
#include <cstdint>
#include <unordered_map>
#include <string>

class Connection{
    private:
    std::string client_addr;
    std::uint16_t client_port;
    public:
    Connection (std::string client_addr, std::uint16_t port) {
        this->client_addr = client_addr;
        this->client_port = port;
    }
};

class Server {
    private:
    int socket_fd;
    std::unordered_map<int, Connection> connection_map;

    public:
    int initializeServer();
    int shutdownServer();
    int startListening();
    int stopServer();
};