#include "server.h"

int main() {
    Server tcpServer;
    tcpServer.initializeServer();
    tcpServer.beginServerListening();
    return 0;
}