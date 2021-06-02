#ifndef NET_H_
#define NET_H_

#include "mbed.h"

namespace smartcon
{
    namespace net {
    
        NetworkInterface * get_network_interface();

        TCPSocket * get_socket();

        void connect();

        void disconnect();

        void socket_connect();
    
    }
}

#endif