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
        Connection newConnection(addr_cpp_string, ntohs(client_addr.sin_port));
        std::cout << "Client connected. ip: " << newConnection.getAddress() << " port: " << newConnection.getPort() << "\n";

        std::string writeBuf = "Hello from server";
        char readBuf[32]{};
        read(client_fd, readBuf, 32);
        std::string readStr(readBuf);
        std::cout << "Client says: " << readStr << "\n";
        write(client_fd, writeBuf.c_str(), 18);
        close(client_fd);
    }
}

std::string Connection::getAddress() {
    return this->client_addr;
}

int Connection::getPort() {
    return this->client_port;
}

