#pragma once

#include <gbase/net/socket_fwd.hpp>
#include <gbase/types.hpp>

#include <netinet/in.h>
#include <sys/socket.h>

namespace gb {
template <domain_type _domain, socket_type_type _type, protocol_type _protocol>
class basic_socket_buf : public std::streambuf {
public:
    explicit basic_socket_buf(basic_socket<_domain, _type, _protocol>& socket)
        : _socket(socket)
    {
        this->_r_buf = new char[DEFAULT_SOCKET_BUFFER_SIZE];
        this->_w_buf = new char[DEFAULT_SOCKET_BUFFER_SIZE];

        setg(this->_r_buf, this->_r_buf, this->_r_buf);
        setp(this->_w_buf, this->_w_buf + DEFAULT_SOCKET_BUFFER_SIZE);
    }

    ~basic_socket_buf() override
    {
        sync();
        delete[] this->_r_buf;
        delete[] this->_w_buf;
    }

    // TODO: safer implement
    // release socket and shutdown stream
    // socket&& release();

protected:
    // for input (or read)
    int underflow() override
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

    // for output (or write)
    int sync() override
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

    int overflow(int c) override
    {
        if (sync() == EOF) {
            return EOF;
        }

        if (c != EOF) {
            sputc(c);
        }
        return c;
    }

private:
    basic_socket<_domain, _type, _protocol>& _socket;
    char* _r_buf = nullptr;
    char* _w_buf = nullptr;
};
} // namespace gb
