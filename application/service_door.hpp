#ifndef SERVICE_DOOR_HPP
#define SERVICE_DOOR_HPP

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include <Callback.h>

class ServiceDoor
{
public:
    ServiceDoor(int sensor_pin, bool opened_state);
    ~ServiceDoor(){}

public:
    void init();

    bool is_opened() const { return is_opened; }

    void tick();

    void attachOnOpenedEvent(const Slot<bool>& slot)
    {
        onOpened_.attach(slot);
    }

    void detachOnOpenedEvent(const Slot<bool>& slot)
    {
        onOpened_.detach(slot);
    }

    void attachOnClosedEvent(const Slot<bool>& slot)
    {
        onClosed_.attach(slot);
    }

    void detachOnClosedEvent(const Slot<bool>& slot)
    {
        onClosed_.detach(slot);
    }

private:
    bool pin_state();

private:
    bool isOpened_;
    bool opened_state_;
    unsigned long debounceTimeCounter_;
    int sensorPin_;

    Signal<bool> onOpened_;
    Signal<bool> onClosed_;
};

#endif // ! SERVICE_DOOR_HPP