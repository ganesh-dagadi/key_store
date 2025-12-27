#include "server.h"
#include "serverErrors.h"
#include "parser.h"
#include "engine.h"
#include <sys/socket.h>
#include <cerrno>
#include <system_error>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <memory>


void printError() {
    std::error_code ec(errno, std::generic_category());
    std::cerr << "Error (" << ec.value() << "): " << ec.message() << '\n';
}

int Connection::readExact(void* buf_ptr, int bytes_to_read) {
    int bytes_read = 0;
    char* buf = static_cast<char*>(buf_ptr);
    while (bytes_read < bytes_to_read) {
        int curr_read = read(this->getConnectionFd(), buf + bytes_read, bytes_to_read - bytes_read);
        if(curr_read < 0) {
            if (errno == EINTR) continue;
            std::cerr << "Error reading from socket for client " << this->getAddress() << "\n";
            printError();
            return -1;
        }

        if (curr_read == 0) {
            std::cerr << "Connection with " << this->getAddress() << " closed before complete read\n";
            throw ConnectionException("Connection closed while reading");
        }
        bytes_read += curr_read;
    }
    return bytes_read;
}

int Connection::writeExact(void* buf_ptr, int bytes_to_write) {
    int bytes_written = 0;
    char* buf = static_cast<char*>(buf_ptr);
    while (bytes_written < bytes_to_write) {
        int curr_read = write(this->getConnectionFd(), buf + bytes_written, bytes_to_write - bytes_written);
        if(curr_read < 0) {
            if (errno == EINTR) continue;
            std::cerr << "Error reading from socket for client " << this->getAddress() << "\n";
            printError();
            return -1;
        }

        if (curr_read == 0) {
            std::cerr << "Connection with " << this->getAddress() << " closed before complete read\n";
            return bytes_written;            
        }
        bytes_written += curr_read;
    }
    return bytes_written;
}

int Server::shutdownServer() {
    close(socket_fd);
    return 0;
}
int Server::initializeServer() {
    // support only IPv4 now. Future with IPv6
    // TODO: read from configuration
    bound_port = 8080;
    bound_ip = "0.0.0.0";
    sa_family_t ip_version = AF_INET;
    bool isReuseAddressEnabled = true;
    socket_fd = socket(ip_version, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        std::cerr << "Socket creation failed \n";
        printError();
        return -1;
    }

    if (isReuseAddressEnabled) {
        int val = 1;
        if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1) {
            std::cerr << "Unable to set reuse address for socket.\n";
            printError();
        }
    }

    struct sockaddr_in addr_meta{};
    addr_meta.sin_family = ip_version;
    addr_meta.sin_port = htons(bound_port);
    const char* ip_addr_copy = bound_ip.c_str();
    short conversion_res = inet_pton(AF_INET, ip_addr_copy, &addr_meta.sin_addr);
    if (conversion_res == 0) {
        std::cerr << "Invalid IP address \n";
        shutdownServer();
        return -1;
    } else if (conversion_res == -1) {
        printError();
        shutdownServer();
        return -1;
    }
    if (bind(socket_fd, (struct sockaddr*)&addr_meta, sizeof(addr_meta)) == -1) {
        std::cerr << "Failed to bind to address \n";
        printError();
        shutdownServer();
        return -1;
    }

    std::cout << "Successfully initalized server on ip: " << bound_ip << " port: " << bound_port << "\n";
    return 0;
}

int Server::beginServerListening() {
    if(listen(socket_fd, SOMAXCONN) == -1) {
        std::cerr << "Listen on ip: " << bound_ip << " port: " << bound_port << "failed \n";
        printError();
        return -1;
    }

    while (true) {
        struct sockaddr_in client_addr{};
        socklen_t size = sizeof(client_addr);
        int client_fd = accept(socket_fd, (struct sockaddr* )&client_addr, &size);
        if (client_fd < 0) {
            printf("Failed to accept connection \n");
            printError();
            continue;
        }
        //TODO: handle connection.
        char addr_str[IPV4_MAX_STR_LEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), addr_str, IPV4_MAX_STR_LEN);
        std::string addr_cpp_string(addr_str);
        Connection newConnection(addr_cpp_string, ntohs(client_addr.sin_port), client_fd);
        std::cout << "Client connected. ip: " << newConnection.getAddress() << " port: " << newConnection.getPort() << "\n";

        while (true) {
            try {
                Command request_command = Parser::parse(newConnection);
                if(request_command.op_code == Opcode::SET) std::cout << "SET command" << std::endl;
                if(request_command.op_code == Opcode::GET) std::cout << "GET command" << std::endl;
                if(request_command.set_ttl) std::cout << "TTL is set to " << request_command.ttl << std::endl;
                else std::cout << "TTL not set" << std::endl;
                std::cout << request_command.key << "is key \n";
                std::cout << request_command.value << "is value \n";
            } catch(const ConnectionException& e) {
                std::cerr << "Connection closed with " << newConnection.getAddress() << "\n";
                break;
            }
            catch (const ParsingError& e) {
                std::cerr << e.what() << std::endl;
                std::string res_body = "Error Parsing the Request";
                Parser::returnResponse(400, res_body.data(), res_body.size(),newConnection);
            }
        }
    }
}

std::string Connection::getAddress() {
    return this->client_addr;
}

int Connection::getPort() {
    return this->client_port;
}

int Connection::getConnectionFd() {
    return this->connection_fd;
}

