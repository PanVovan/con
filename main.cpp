
#include "mbed.h"
#include "hcsr04ultrasonic.h"
#include "MQTT-CLIENT/MQTTService.h"
#include <string>


void handler(float dist);
HCSR04 sensor(D7, D6, 1, handler);


void handler(float dist)
{
    string str = to_string(dist);
    SendDataToMQTT(CreateTopicName("/fullness"), str.c_str());
}

void AnalyseFullness(const char * buffer, float fullness)
{
    string str = to_string(fullness);
    buffer = str.c_str();
}

void AnalyseCoords(const char * buffer, float fullness)
{
    string str = to_string(fullness);
    buffer = str.c_str();
}

int main()
{
    ConnectToMQTT();
    const char* buffer = new char[100];
    AnalyseFullness(buffer, sensor.GetDistance());
    SendDataToMQTT(CreateTopicName("/fullness"), buffer);
    AnalyseCoords(buffer, 51.508);
    SendDataToMQTT(CreateTopicName("/longitude"), buffer);
    AnalyseCoords(buffer, -0.11);
    SendDataToMQTT(CreateTopicName("/latitude"), buffer);


    while (1)
    {
        sensor.CallHandlerIfDistanseChanged();
        wait_us(60000);
    }
}