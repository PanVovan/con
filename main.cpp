
#include "mbed.h"
#include "HCSR04_driver/hcsr04ultrasonic.h"
#include "MQTT-CLIENT/MQTTService.h"
#include "TextLCD.h"
#include <cstdio>
#include <string>

const char * buffer = new char[100];

void AnalyseFullness(float fullness);
HCSR04 sensor(D9, D10, 1.0f, AnalyseFullness);
TextLCD lcd(D12, D11, D7, D6, D5, D4);


void AnalyseFullness(float fullness)
{
    float max = sensor.GetMax();
    int percent = 100 - (fullness-5)/(max-5)*100;
    sprintf((char*)buffer, "%d", percent);
    
    SendDataToMQTT(CREATE_CLIENT_TOPIC_NAME(fullness), buffer);
    lcd.locate(0, 0);
    lcd.printf("Fullness: %6d", percent);
}

void AnalyseCoords(const char * buffer, float coord)
{
    sprintf((char*)buffer, "%f", coord);
    lcd.locate(0, 1);
    lcd.printf("Lon: %.4f", coord);

}

void init()
{
    AnalyseFullness(sensor.GetDistance());
    AnalyseCoords(buffer, 51.508);
    SendDataToMQTT(CREATE_CLIENT_TOPIC_NAME(longitude), buffer);
    AnalyseCoords(buffer, -0.11);
    SendDataToMQTT(CREATE_CLIENT_TOPIC_NAME(latitude), buffer);
}

void init_hardware(MQTT::MessageData &data)
{
    if (strcmp((const char*)(data.message.payload), "true"))
    {
        init();
    }
}

int main()
{
    lcd.cls();

    sensor.SetMax(25.0f);
    sensor.Start();
    ConnectToMQTT(init_hardware);
    init();
    
    while (1)
    {
        sensor.CallHandlerIfDistanseChanged();
    }
}