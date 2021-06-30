
#include "mbed.h"
#include "HCSR04_driver/hcsr04ultrasonic.h"
#include "MQTT-CLIENT/MQTTService.h"
#include "TextLCD.h"
#include <cstdio>
#include <string>
#include <amperka-gps.h>

const char * buffer = new char[100];

void AnalyseFullness(float fullness);
void AnalyseLon(float coord);
void AnalyseLat(float coord);

HCSR04 sensor(D9, D10, 1.0f, AnalyseFullness);
TextLCD lcd(D11, D12, D4, D5, D6, D7);
AmperkaGPS gps(D8, D2, AnalyseLat, AnalyseLon);
Thread LCDThread;

float _coord;
bool trig;


void AnalyseFullness(float fullness)
{
    float max = sensor.GetMax();
    int percent = 100 - (fullness-5)/(max-5)*100;
    sprintf((char*)buffer, "%d", percent);
    
    lcd.locate(0, 0);
    lcd.printf("Fullness: %5d%s", percent, "%");
    SendDataToMQTT(CREATE_CLIENT_TOPIC_NAME(fullness), buffer);
    
}

void AnalyseLon(float coord)
{
    sprintf((char*)buffer, "%f", coord);
    SendDataToMQTT(CREATE_CLIENT_TOPIC_NAME(longitude), buffer);
}

void AnalyseLat(float coord)
{
    sprintf((char*)buffer, "%f", coord);
    SendDataToMQTT(CREATE_CLIENT_TOPIC_NAME(latitude), buffer);
}

void init()
{
    AnalyseFullness(sensor.GetDistance());
    AnalyseLon(gps.GetLongitude());
    AnalyseLat(gps.GetLatitude());
    
    
}

void init_hardware(MQTT::MessageData &data)
{
    if (strcmp((const char*)(data.message.payload), "true"))
    {
        init();
    }
}

void LCD()
{
    while (true)
    {
        lcd.locate(0, 1);
        if(trig)
        {
            _coord = gps.GetLatitude();
            lcd.printf("Lat: %.4f", _coord);
        }
        else
        {
            _coord = gps.GetLongitude();
            lcd.printf("Lon: %.4f", _coord);
        }
        trig = !trig;
        wait_us(5'000'000);
    }
}

int main()
{
    lcd.cls();

    sensor.SetMax(25.0f);
    sensor.Start();
    ConnectToMQTT(init_hardware);
    init();
    LCDThread.start(callback(LCD));
    
    while (1)
    {
        sensor.CallHandlerIfDistanseChanged();
        gps.CallIfCoordChanged();
    }
}