#include "hcsr04ultrasonic.h"

HCSR04::HCSR04(PinName echo,
               PinName trig,
               float fault,
               OnDistanceChanged handler)
            :
            _echo(echo),
            _trig(trig),
            _fault(fault),
            _onDistanceChanged(handler)
{
    _distance = -1;
    _previous = -1; 
}

void HCSR04::Start()
{
    _echo.rise(callback(this, &HCSR04::startTimer));
    _echo.fall(callback(this, &HCSR04::stopTimer));
    _thread.start(callback(this, &HCSR04::measure));
}

void HCSR04::startTimer()
{   
    _timer.reset();
    _timer.start();
}

void HCSR04::stopTimer()
{
    _timer.stop();
    _end =_timer.read_us();
}


float HCSR04::GetDistance()
{
    return _distance; 
}

void HCSR04::SetOnDistanceChangedHandler(OnDistanceChanged handler)
{
    _onDistanceChanged = handler;
}

bool HCSR04::IsDistanceChanged()
{
    return fabs(_previous - _distance) > _fault;
}

void HCSR04::measure()
{
    while(1)
    {
        _trig = 1;
        wait_us(20);
        _trig = 0;
        wait_us(50000);

        _previous = _distance;

        _distance = _end/2.0f/29.412f;

        wait_us(1000);
    }
}