#include "../include/client.h"
#include <sys/socket.h>
#include <cerrno>
#include <system_error>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void printError(const std::error_code ec, const std::string& message) {
     std::cerr << "Error(" << ec << "): " << message << std::endl;
}

void Client::connectToServer() {
    socket_fd = socket(domain, type, 0);

    if (Client::socket_fd < 0) {
        // Error while initiating a network communication
        const std::error_code ec (errno, std::generic_category());
        printError(ec, "Error while initiating a network communication");
    }

    struct sockaddr_in addr = {};
    addr.sin_family = domain;
    addr.sin_port = SERVER_PORT_NUMBER;
    addr.sin_addr.s_addr = SERVER_IP_ADDRESS;

    connection_fd = connect(socket_fd, reinterpret_cast<const struct sockaddr *>(&addr), sizeof(addr));

    if (connection_fd < 0) {
        // Error while connecting to the remote server
        const std::error_code ec (errno, std::generic_category());
        printError(ec, "Error while connecting to the remote server");
    }
}

void Client::disconnectFromServer() {
    close(socket_fd);
    socket_fd = -1;
    connection_fd = -1;
}


int32_t Client::readFull(uint8_t *buffer, size_t len) const {
    while (len > 0) {
        const ssize_t numberOfBytes = read(socket_fd, buffer, len);
        if (numberOfBytes <= 0) {
            // Error while reading data from the TCP buffer
            const std::error_code ec (errno, std::generic_category());
            printError(ec, "Error while reading data from TCP buffer");
        }
        const auto numberOfBytesSigned = static_cast<size_t>(numberOfBytes);
        if (numberOfBytesSigned > len) {
            const std::error_code ec (errno, std::generic_category());
            printError(ec, "The number bytes read, is more than expected");
        }
        len -= numberOfBytesSigned;
        buffer += numberOfBytesSigned;
    }
    return 0;
}

int32_t Client::writeFull(const uint8_t *buffer, size_t len) const {
    while (len > 0) {
        const ssize_t numberOfBytes = write(socket_fd, buffer, len);
        if (numberOfBytes <= 0) {
            // Error while writing data to the TCP buffer
            const std::error_code ec (errno, std::generic_category());
            printError(ec, "Error while writing data to TCP buffer");
        }
        const auto numberOfBytesSigned = static_cast<size_t>(numberOfBytes);
        if (numberOfBytesSigned > len) {
            const std::error_code ec (errno, std::generic_category());
            printError(ec, "The number bytes written, is more than expected");
        }
        len -= numberOfBytesSigned;
        buffer += numberOfBytesSigned;
    }
    return 0;
}

int Client::getSocketFileDescriptor() const {
    return this->socket_fd;
}

int Client::connectionFileDescriptor() const {
    return this->connection_fd;
}
