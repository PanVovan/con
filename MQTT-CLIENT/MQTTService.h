
#ifndef MQTTService_H_
#define MQTTService_H_

#include "mbed.h"
#include <MQTTClientMbedOs.h>




void ConnectToMQTT();

void SendDataToMQTT(const char * topic, const char * data);

const char *CreateTopicName(const char*);




#endif