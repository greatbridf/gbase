#include <gbase/net/socket.hpp>

#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

namespace gb {
std::string socket::ip() const
{
    return _ip;
}

port_t socket::port() const
{
    return _port;
}

socket::socket()
    : basic_socket()
{
}

socket::socket(const char* ip, port_t port)
    : socket()
{
    if (ip == nullptr || ::strcmp(ip, "") == 0) {
        ip = SOCKET_ADDR_ANY;
    }
    make_addr(ip, port);
}

socket::socket(int socket, sockaddr_in addr)
    : basic_socket(socket, *(sockaddr*)&addr)
{
    _ip = inet_ntoa(addr.sin_addr);
    _port = ntohs(addr.sin_port);
}

void socket::make_addr(const char* ip, port_t port)
{
    sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    _addr = *(sockaddr*)&addr;
    _ip = ip;
    _port = port;
}

client_socket::client_socket(const char* ip, port_t port)
    : socket(ip, port)
{
}

client_socket::client_socket(int __socket, sockaddr_in addr)
    : socket(__socket, addr)
{
}

bool client_socket::connect()
{
    if (_state != READY)
        return false;
    auto result = ::connect(_socket, &_addr, sizeof(sockaddr_in));
    if (result == 0) {
        _state = CONNECTED;
        return true;
    }
    return false;
}

server_socket::server_socket(port_t port)
    : server_socket("0.0.0.0", port)
{
}

server_socket::server_socket(const char* ip, port_t port)
    : socket(ip, port)
{
    bind();
}

bool server_socket::bind()
{
    auto result = ::bind(_socket, &_addr, sizeof(sockaddr_in));
    return result == 0;
}

bool server_socket::listen()
{
    if (_state != READY)
        return false;
    auto result = ::listen(_socket, max_queue_length);
    if (result == 0) {
        _state = LISTENING;
        return true;
    }
    return false;
}

std::unique_ptr<client_socket> server_socket::accept()
{
    if (_state != LISTENING)
        return nullptr;
    sockaddr_in c_addr = {};
    socklen_t len = sizeof(sockaddr_in);

    int socket = ::accept(_socket, (sockaddr*)&c_addr, &len);
    if (socket < 0) {
        if (errno == SOCKET_ERROR_CLOSED)
            close();
        return nullptr;
    }
    auto ptr = std::unique_ptr<client_socket>(new client_socket(socket, c_addr));
    ptr->_state = CONNECTED;
    return ptr;
}
}
