#include "tcp_socket.h"

#include <iostream>
#include <string>

#include "WinSock2.h"
#include "WS2tcpip.h"

namespace pear::net::tcp {

    int SocketConnect(const char *host, int port) {
        std::cout << "creating socket " << host << ":" << port << std::endl;
        WSADATA wsa_data;
        auto result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
        if (result != 0) {
            return FAILED;
        }

        addrinfo hints{};
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        struct addrinfo *addr_info_result = nullptr;
        result = getaddrinfo(host, std::to_string(port).c_str(), &hints, &addr_info_result);
        if (result != 0) {
            std::cout << "getaddrinfo error " << result << std::endl;
            freeaddrinfo(addr_info_result);
            WSACleanup();
            return FAILED;
        }

        auto socket_connection = INVALID_SOCKET;

        socket_connection = socket(addr_info_result->ai_family, addr_info_result->ai_socktype,
                                   addr_info_result->ai_protocol);
        if (socket_connection == INVALID_SOCKET) {
            std::cout << "create socket error " << SocketError() << std::endl;
            freeaddrinfo(addr_info_result);
            WSACleanup();
            return FAILED;
        }

        result = connect(socket_connection, addr_info_result->ai_addr, (int) addr_info_result->ai_addrlen);
        if (result == SOCKET_ERROR) {
            std::cout << "connect failed " << host << ":" << port << " " << SocketError() << std::endl;
            closesocket(socket_connection);
            socket_connection = INVALID_SOCKET;
        }

        freeaddrinfo(addr_info_result);

        if (socket_connection == INVALID_SOCKET) {
            std::cout << "unable to connect " << host << ":" << port << std::endl;
            WSACleanup();
            return RECOVERABLE;
        }

        return socket_connection;
    }

    int SocketRead(int socket, char *read_buf, int read_lim) {
        return recv(socket, read_buf, read_lim, 0);
    }

    int SocketSend(int socket, const char *data, const int data_len) {
        return send(socket, data, data_len, 0);
    }

    int SocketError() {
        return WSAGetLastError();
    }

    void CloseSocket(int socket) {
        closesocket(socket);
    }
}
