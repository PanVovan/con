#ifndef AmperkaGPS_H_
#define AmperkaGPS_H_
#include <SerialGPS.h>
#include <cstdio>

#include "mbed.h"

class AmperkaGPS
{
    SerialGPS _gps;
    Thread _thread;

    float _lon, _lat;
    bool _lonch, _latch;

    void (*onLatCh)(float), (*onLonCh)(float);

    void measure()
    {
        while (true)
        {
            if (_gps.sample())
            {
                printf("sample()\n");
                if (_lat != _gps.latitude)
                {
                    _lat = _gps.latitude;
                    _latch = true;
                }
                if (_lon != _gps.longitude)
                {
                    _lon = _gps.longitude;
                    _lonch = true;
                }
            }
        }
    }

public:
    AmperkaGPS(PinName tx, PinName rx, void (*onLatCh)(float), void (*onLonCh)(float))
    :
    _gps(tx, rx, 115200),
    onLatCh(onLatCh),
    onLonCh(onLonCh)
    {
        _thread.start(callback(this, &AmperkaGPS::measure));
        _lat = _gps.latitude;
        _lon = _gps.longitude;
    }

    void CallIfCoordChanged()
    {
        if (_latch)
        {
            onLatCh(_lat);
            _latch = false;
        }
        if (_lonch)
        {
            onLonCh(_lon);
            _lonch = false;
        }
    }

    float GetLongitude()
    {
        _lonch = false;
        return _lon;
    }

    float GetLatitude()
    {
        _latch = false;
        return _lat;

    }

    ~AmperkaGPS()
    {
        _thread.join();
    }
};
#endif