#pragma once

#include <gbase/net/socket_fwd.hpp>
#include <gbase/types.hpp>

#include <sys/socket.h>
#include <unistd.h>
namespace gb {
template <domain_type _domain, socket_type_type _type, protocol_type _protocol>
class basic_socket : public non_copyable {
public:
    ~basic_socket()
    {
        close();
    }

    sockaddr* const addr() const
    {
        return (sockaddr* const) & _addr;
    }

    bool set_timeout(time_t timeout)
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

    time_t timeout() const
    {
        return _timeout;
    }

    socket_states state() const
    {
        return _state;
    }

    void close()
    {
        if (_state == CLOSED)
            return;
        ::close(_socket);
        _state = CLOSED;
    }

    friend socket_buf;

protected:
    basic_socket()
        : _addr(sockaddr())
        , _timeout(DEFAULT_SOCKET_TIMEOUT)
        , _state(READY)
    {
        _socket = ::socket(_domain, _type, _protocol);
        set_timeout(_timeout);
    }

    basic_socket(socket_type socket, sockaddr addr)
        : _socket(socket)
        , _addr(addr)
        , _timeout(DEFAULT_SOCKET_TIMEOUT)
    {
        set_timeout(_timeout);
    }

    socket_type _socket;

    sockaddr _addr;

    time_t _timeout;

    socket_states _state;
};
}
