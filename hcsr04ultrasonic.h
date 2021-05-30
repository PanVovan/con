#pragma once
#include "mbed.h"

using OnDistanceChanged = void(*) (float);

class HCSR04
{
private:
    
    float _distance, _previous, _end;
    float _fault;

    /*
     * IO, used for measured echo time.
     */

    InterruptIn _echo;
    DigitalOut _trig;
    
    /*
     * used for checking distance (s = v*t, where v - speed of sound,
     * t - value of timer).
     */
    Timer _timer;
    Thread _thread;
    void stopTimer();
    void startTimer();
    void measure();


    /*
     * reference on handler, used for process situation, when distance changed.
     */
    OnDistanceChanged _onDistanceChanged;

    

public:
    HCSR04(PinName echo,
            PinName trig,
            float fault = 0.0,
            OnDistanceChanged handler = nullptr);
    
    void Start();

    void SetOnDistanceChangedHandler(OnDistanceChanged);
    float GetDistance();

    bool IsDistanceChanged();

    void CallHandlerIfDistanseChanged()
    {
        if(IsDistanceChanged())
        {
            (*_onDistanceChanged)(_distance);
        }
    }
};