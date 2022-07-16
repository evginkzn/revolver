#include "service_door.hpp"

ServiceDoor::ServiceDoor(int sensor_pin, bool opened_state)
    : sensorPin_(sensor_pin)
    , opened_state_(opened_state)
{
}

void ServiceDoor::init()
{
    pinMode(sensorPin_, INPUT);
}

void ServiceDoor::tick()
{
    if (pin_state() == opened_state_)
    {
        if (is_opened)
        {
            debounceTimeCounter_ = millis();
        }
        else
        {
            if (millis() - debounceTimeCounter_ >= 2000)
            {
                onClosed_.fire(false);
            }
        }
    }
    else
    {
        if (is_opened)
        {
            if (millis() - debounceTimeCounter_ >= 2000)
            {
                onOpened_.fire(false);
            }
        }
        else
        {
            debounceTimeCounter_ = millis();
        }
    }
}

bool ServiceDoor::pin_state()
{
    return digitalRead(sensorPin_) == HIGH ? true : false;
}