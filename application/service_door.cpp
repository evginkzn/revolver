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
        if (isOpened_)
        {
            debounceTimeCounter_ = millis();
        }
        else
        {
            if (millis() - debounceTimeCounter_ >= 2000)
            {
                isOpened_ = true;
                onOpened_.fire(isOpened_);
            }
        }
    }
    else
    {
        if (!isOpened_)
        {
            debounceTimeCounter_ = millis();
        }
        else
        {
            if (millis() - debounceTimeCounter_ >= 2000)
            {
                isOpened_ = false;
                onClosed_.fire(isOpened_);
            }
        }
    }
}

bool ServiceDoor::pin_state()
{
    return digitalRead(sensorPin_) == HIGH ? true : false;
}