#ifndef AmperkaGPS_H_
#define AmperkaGPS_H_
#include <GPS.h>
#include <cstdio>

#include "mbed.h"

class AmperkaGPS
{
    GPS _gps;
    Thread _thread;

    int _lon, _lat;
    bool _lonch, _latch;

    void (*onLatCh)(float), (*onLonCh)(float);

    void measure()
    {
        while (true)
        {
            if (_gps.sample())
            {
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
            wait_us(1'000'000);
        }
    }

public:
    AmperkaGPS(PinName tx, PinName rx, void (*onLatCh)(float), void (*onLonCh)(float))
    :
    _gps(tx, rx),
    onLatCh(onLatCh),
    onLonCh(onLonCh)
    {
        _thread.start(callback(this, &AmperkaGPS::measure));
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