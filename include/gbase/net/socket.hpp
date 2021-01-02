#pragma once

/*
 *  tcp ipv4 socket
 *  and unified socket
 */

#include <netinet/in.h>
#include <sys/socket.h>

#include <memory>
#include <string>

#include <gbase/net/basic_socket.hpp>
#include <gbase/types.hpp>

namespace gb {
// tcp ipv4 socket base
class socket : public basic_socket<AF_INET, SOCK_STREAM, IPPROTO_TCP> {
public:
    std::string ip() const;
    port_t port() const;

protected:
    socket();
    socket(const char* ip, port_t port);
    socket(int socket, sockaddr_in addr);

    std::string _ip;
    port_t _port;

    void make_addr(const char* ip, port_t port);
};

// tcp ipv4 client socket
class client_socket : public socket {
public:
    client_socket(const char* ip, port_t port);
    client_socket(int socket, sockaddr_in addr);

    bool connect();

    friend class server_socket;
};

// tcp ipv4 server socket
class server_socket : public socket {
public:
    explicit server_socket(port_t port);
    server_socket(const char* ip, port_t port);

    bool listen();
    std::unique_ptr<client_socket> accept();

    size_t max_queue_length = DEFAULT_SERVER_SOCKET_QUEUE_LENGTH;

private:
    bool bind();
};
}
