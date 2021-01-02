#pragma once

#include <gbase/net/socket_types.hpp>

#include <netinet/in.h>
#include <sys/socket.h>

namespace gb {

template <domain_type _domain, socket_type_type _type, protocol_type _protocol>
class basic_socket;

template <domain_type _domain, socket_type_type _type, protocol_type _protocol>
class basic_socket_buf;

typedef basic_socket_buf<AF_INET, SOCK_STREAM, IPPROTO_TCP> socket_buf;

} // namespace gb
