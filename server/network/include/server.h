#pragma once
#include <cstdint>
#include <unordered_map>
#include <string>

#define IPV4_MAX_STR_LEN 16

// std::ostream& operator<<(std::ostream& out, const Opcode value) {
//     switch (value) {
//         case Opcode::SET: return out << "SET";
//         case Opcode::GET: return out << "GET";
//         case Opcode::SET_TTL: return out << "SET_TTL";
//         case Opcode::GET_TTL: return out << "GET_TTL";
//         case Opcode::STAT: return out << "STAT";
//          default: return out << "Unknown opcode";
//     }
// }

enum class HeaderVersion : uint8_t{
    V1,
    V2
};
// std::ostream& operator<<(std::ostream& out, const HeaderVersion value) {
//     switch (value) {
//         case HeaderVersion::V1: return out << "version 1";
//         case HeaderVersion::V2: return out << "version 2";
//          default: return out << "Unknown Version";
//     }
// }

class Connection{
    private:
    std::string client_addr;
    std::uint16_t client_port;
    int connection_fd;
    public:
    Connection (std::string client_addr, std::uint16_t port, int connection_fd) {
        this->client_addr = client_addr;
        this->connection_fd = connection_fd;
        this->client_port = port;
    }

    std::string getAddress();
    int readExact(void* buf_ptr, int bytes_to_read);
    int writeExact(void* buf_ptr, int bytes_to_write);
    int getPort();
    int getConnectionFd();
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