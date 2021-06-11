
#ifndef MQTTService_H_
#define MQTTService_H_

#include "mbed.h"
#include <MQTTClientMbedOs.h>
#include <MQTTConnectionConfig.h>

#define CREATE_CLIENT_TOPIC_NAME(topic_name) "containers/" MQTT_CLIENT_ID "/"#topic_name

void ConnectToMQTT( void (*init_function)(MQTT::MessageData &) = nullptr);

void SendDataToMQTT(const char * topic, const char * data);

#endif