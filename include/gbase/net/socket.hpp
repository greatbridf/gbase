#pragma once

/*
 *  tcp ipv4 socket
 *  and unified socket
 */

#include <netinet/in.h>
#include <sys/socket.h>

#include <memory>
#include <string>

#include <gbase/types.hpp>

namespace gb {
class basic_socket : public non_copyable {
public:
    ~basic_socket();

    sockaddr* const addr() const;

    bool set_timeout(time_t timeout);
    time_t timeout() const;

    socket_states state() const;

    void close();

    friend class socket_buf;

protected:
    basic_socket(int domain, int type, int protocol);
    basic_socket(int domain, int type, int protocol, int socket, sockaddr addr);

    int _socket;

    int _domain;
    int _type;
    int _protocol;
    sockaddr _addr = sockaddr();

    time_t _timeout = DEFAULT_SOCKET_TIMEOUT;

    socket_states _state = READY;
};

// tcp ipv4 socket base
class socket : public basic_socket {
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

// tcp ipv4 socket buffer
class socket_buf : public std::streambuf {
public:
    explicit socket_buf(basic_socket& socket);
    ~socket_buf() override;

    // TODO: safer implement
    // release socket and shutdown stream
    // socket&& release();

protected:
    // for input (or read)
    int underflow() override;

    // for output (or write)
    int sync() override;
    int overflow(int c) override;

private:
    basic_socket& _socket;
    char* _r_buf = nullptr;
    char* _w_buf = nullptr;
};
}
