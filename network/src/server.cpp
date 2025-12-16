#include "server.h"
#include <sys/socket.h>
#include <cerrno>
#include <system_error>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void printError() {
    std::error_code ec(errno, std::generic_category());
    std::cerr << "Error (" << ec.value() << "): " << ec.message() << '\n';
}

int Server::shutdownServer() {
    close(socket_fd);
    return 0;
}
int Server::initializeServer() {
    // support only IPv4 now. Future with IPv6
    // TODO: read from configuration
    uint16_t port = 8080;
    std::string ip_addr = "0.0.0.0";
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
    addr_meta.sin_port = htons(port);
    const char* ip_addr_copy = ip_addr.c_str();
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

    std::cout << "Successfully initalized server on ip: " << ip_addr << " port: " << port << "\n";
    return 0;
}

