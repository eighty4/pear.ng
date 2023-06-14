#ifndef PEAR_NETWORKING_SOCKET_CONNECTION_H
#define PEAR_NETWORKING_SOCKET_CONNECTION_H

namespace pear::net::tcp {

    const int FAILED = -1;

    const int RECOVERABLE = -2;

    int SocketConnect(const char *host, int port);

    int SocketRead(int socket, char *read_buf, int read_lim);

    int SocketSend(int socket, const char *bytes, int bytes_len);

    int SocketError();

    void CloseSocket(int socket);

}

#endif // PEAR_NETWORKING_SOCKET_CONNECTION_H
