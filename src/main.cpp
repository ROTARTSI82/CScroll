#include "csc/tcp_server.hpp"
#include "csc/log.hpp"
#include "csc/varint.hpp"

int main() {
    csc::varIntSanityCheck();
    return 0;
    csc::TcpServer serv{"127.0.0.1", "25565"};
    while (true) {
        serv.tickAccept();
        serv.tickClients(3);
    }
}