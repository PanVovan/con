#include "Network.h"
#include <MQTTConnectionConfig.h>


namespace smartcon
{
    namespace net {

        WiFiInterface *net;
        SocketAddress addr;
        TCPSocket socket;
    
        NetworkInterface * get_network_interface()
        {
            return net;
        }

        TCPSocket * get_socket()
        {
            return &socket;
        }

        void socket_connect()
        {
            net->gethostbyname(MQTT_HOST, &addr);
            addr.set_port(MQTT_PORT);
            int res = socket.connect(addr);
            if (res < 0)
            {
                printf("socket.connect failed with return value: %d\n", res);
            }
            else
            {
                printf("socket.connect success: %d\n", res);
            }

        }  

        void connect()
        {
            net = WiFiInterface::get_default_instance();

            #ifdef DEBUG
            if (!wifi) {
                printf("ERROR: No WiFiInterface found.\n");
                return;
            }

            printf("\nConnecting to %s...\n", MBED_CONF_APP_WIFI_SSID);
            #endif

            int ret = net->connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
            #ifdef DEBUG
            if (ret != 0) {
                printf("\nConnection error: %d\n", ret);
            }
            else {
                printf("\nConnection success: %d\n", ret);
            }
            #endif

            net->gethostbyname(MQTT_HOST, &addr);
            addr.set_port(MQTT_PORT);

            int res = socket.open(net);
            
            if (res < 0)
            {
                printf("socket.open failed with return value: %d\n", res);
            }
            else
            {
                printf("socket.open success: %d\n", res);
            }

            res = socket.connect(addr);
            if (res < 0)
            {
                printf("socket.connect failed with return value: %d\n", res);
            }
            else
            {
                printf("socket.connect success: %d\n", res);
            }
        }

        void disconnect()
        {

        }
            
    }
}