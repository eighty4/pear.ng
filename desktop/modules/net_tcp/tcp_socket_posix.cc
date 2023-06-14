#include "tcp_socket.h"

#include <iostream>

#include "arpa/inet.h"
#include "netdb.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "sys/un.h"
#include "unistd.h"

namespace pear::net::tcp {

    int SocketConnect(const char *host, int port) {
        struct sockaddr_in server_address{};
        memset(&server_address, 0, sizeof(server_address));
        server_address.sin_len = sizeof(server_address);
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(port);
        server_address.sin_addr.s_addr = INADDR_ANY;

        int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock == -1) {
            std::cout << "socket creation error " << SocketError() << std::endl;
            return FAILED;
        }
        if (connect(sock, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
            std::cout << "connection error " << SocketError() << std::endl;
            close(sock);
            return FAILED;
        }

        return sock;
    }

    int SocketRead(int socket, char *read_buf, int read_len) {
        return recv(socket, read_buf, read_len, 0);
    }

    int SocketSend(int socket, const char *data, const int data_len) {
        return send(socket, data, data_len, 0);
    }

    int SocketError() {
        return errno;
    }

    void CloseSocket(int socket) {
        close(socket);
    }
}
