#include "MQTTService.h"
#include <cstdio>
#include <cstring>
#include <Network.h>

TCPSocket* socket = smartcon::net::get_socket();
NetworkInterface* i = smartcon::net::get_network_interface();
MQTTClient client(socket);

void ConnectToMQTT( void (*init_function)(MQTT::MessageData &))
{
    smartcon::net::connect();
    MQTTPacket_willOptions last = MQTTPacket_willOptions_initializer;
    last.message.cstring = (char *)MQTT_CLIENT_ID;
    last.topicName.cstring = (char *) MQTT_LAST_WILL_TOPIC;
    
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = (char *)MQTT_CLIENT_ID;
    data.username.cstring = (char *)MQTT_USER;
    data.password.cstring = (char *)MQTT_PASSWORD;
    data.keepAliveInterval = 60;
    data.will = last;

    int res = client.connect(data);

    #ifdef DEBUG
    if (res < 0)
    {
        printf("client.connect failed with return value: %d\n", res);
    }
    else
    {
        printf("client.connect success: %d\n", res);
    }
    #endif

    SendDataToMQTT("containers/new_container", MQTT_CLIENT_ID);
    client.subscribe("server", MQTT::QOS0, init_function);
}

void SendDataToMQTT(const char * topic, const char * data)
{
    MQTT::Message message;
    message.qos = MQTT::QOS0;
    message.retained = false;
    message.dup = false;
    message.payload = (void *)data;
    message.payloadlen = strlen(data);
    int res = client.publish(topic, message);
    #ifdef DEBUG
    if (res < 0)
    {
        printf("socket failed with return value: %d NetIsAvailable: %d\n", res, i != nullptr);
    }
    #endif
    client.yield(1000);
}