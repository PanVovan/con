#include "MQTTService.h"
#include <MQTTSocketTLS.h>
#include <cstdio>
#include <cstring>


WiFiInterface *wifi;
SocketAddress addr;
MQTTSocketTLS socket;
MQTTClient client(&socket);

int scan_demo(WiFiInterface *wifi);
void connect();

    /* get connect info */
const char *username = "tester";
const char *password = "Vovvan41";
const char *clientID = "garbage";
const char *server_topic = "server";
const char *msg;
const char *fullName;

static void init_client (MQTT::MessageData &)
{

}



const char *CreateTopicName(const char * topic)
{
    char* buffer = new char[100];
    strcat(buffer, clientID);
    strcat(buffer, topic);
    return buffer;
}

void ConnectToMQTT()
{
    connect();
    int res = socket.open(wifi);

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


    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = (char *)clientID;

    if (password || MBED_CONF_MBED_MQTT_TESTS_USERNAME_ALWAYS)
    {
        data.username.cstring = (char *)username;
        data.password.cstring = (char *)password;
    }

    client.connect(data);

    client.subscribe(server_topic, MQTT::QOS0, init_client);
}

void SendDataToMQTT(const char * topic, const char * data)
{
    MQTT::Message message;
    message.qos = MQTT::QOS0;
    message.retained = false;
    message.dup = false;
    message.payload = (void *)data;
    message.payloadlen = strlen(data) + 1;
    client.publish(topic, message);
    client.yield(10);
}


void connect()
{
        #ifdef MBED_MAJOR_VERSION
    printf("Mbed OS version %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
#endif

    wifi = WiFiInterface::get_default_instance();
    if (!wifi) {
        printf("ERROR: No WiFiInterface found.\n");
        return;
    }

    printf("\nConnecting to %s...\n", MBED_CONF_APP_WIFI_SSID);
    int ret = wifi->connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
    if (ret != 0) {
        printf("\nConnection error: %d\n", ret);
    }
    else {
        printf("\nConnection success: %d\n", ret);
    }
    wifi->gethostbyname("e9398927ee8e4a2aab1c5e18beb50d0d.s1.eu.hivemq.cloud", &addr);
}