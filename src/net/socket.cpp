#include <gbase/net/socket.hpp>

#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

namespace gb {
basic_socket::basic_socket(int domain, int type, int protocol)
    : _domain(domain)
    , _type(type)
    , _protocol(protocol)
    , _addr(sockaddr())
    , _timeout(DEFAULT_SOCKET_TIMEOUT)
{
    _socket = ::socket(_domain, _type, _protocol);
    set_timeout(_timeout);
}

basic_socket::~basic_socket()
{
    close();
}

sockaddr* const basic_socket::addr() const
{
    return (sockaddr* const) & _addr;
}

bool basic_socket::set_timeout(time_t timeout)
{
    timeval val { timeout, 0 };
    auto result1 = ::setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, &val, sizeof(val));
    auto result2 = ::setsockopt(_socket, SOL_SOCKET, SO_SNDTIMEO, &val, sizeof(val));
    if (result1 == 0 && result2 == 0) {
        _timeout = timeout;
        return true;
    }
    return false;
}

time_t basic_socket::timeout() const
{
    return _timeout;
}

socket_states basic_socket::state() const
{
    return _state;
}

void basic_socket::close()
{
    if (_state == CLOSED) {
        return;
    }
    ::close(_socket);
    _state = CLOSED;
}

basic_socket::basic_socket(int domain, int type, int protocol, int socket, sockaddr addr)
    : _domain(domain)
    , _type(type)
    , _protocol(protocol)
    , _socket(socket)
    , _addr(addr)
    , _timeout(DEFAULT_SOCKET_TIMEOUT)
{
    set_timeout(_timeout);
}

std::string socket::ip() const
{
    return _ip;
}

port_t socket::port() const
{
    return _port;
}

socket::socket()
    : basic_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)
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
    : basic_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP, socket, *(sockaddr*)&addr)
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

socket_buf::socket_buf(basic_socket& socket)
    : _socket(socket)
{
    this->_r_buf = new char[DEFAULT_SOCKET_BUFFER_SIZE];
    this->_w_buf = new char[DEFAULT_SOCKET_BUFFER_SIZE];

    setg(this->_r_buf, this->_r_buf, this->_r_buf);
    setp(this->_w_buf, this->_w_buf + DEFAULT_SOCKET_BUFFER_SIZE);
}

socket_buf::~socket_buf()
{
    delete[] this->_r_buf;
    delete[] this->_w_buf;
}

int socket_buf::underflow()
{
    if (_socket.state() == socket_states::CLOSED)
        return EOF;
    ssize_t n = ::recv(_socket._socket, eback(), DEFAULT_SOCKET_BUFFER_SIZE, 0);

    if (n <= 0) {
        _socket.close();
        return EOF;
    } else {
        setg(eback(), eback(), eback() + n);
    }

    return *eback();
}

int socket_buf::sync()
{
    if (_socket.state() == socket_states::CLOSED)
        return EOF;

    size_t sent = 0;
    size_t total = pptr() - pbase();

    while (sent < total) {
        ssize_t n = ::send(_socket._socket, _w_buf, total - sent, 0);

        if (n > 0) {
            sent += n;
        } else {
            _socket.close();
            return EOF;
        }
    }

    setp(pbase(), pbase() + DEFAULT_SOCKET_BUFFER_SIZE);
    pbump(0);

    return 0;
}

int socket_buf::overflow(int c)
{
    if (sync() == EOF) {
        return EOF;
    }

    if (c != EOF) {
        sputc(c);
    }
    return c;
}
}
