#ifndef CAP_HPP
#define CAP_HPP

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include <Callback.h>
#include <Servo.h>

class Cap
{
private:
    enum Angles
    {
        Servo1ClosedAngle = 10,
        Servo2ClosedAngle = 10,

        Servo1OpenedAngle = 120,
        Servo2OpenedAngle = 120
    };

    enum State
    {
        StateIdle = 0,
        StateFirstServoAction,
        StateSecondServoAction,
        StateSecondServoBack,
        StateFirstServoBack
    };

public:
    Cap();
    ~Cap(){}

public:
    void init(int servo1_pin, int servo2_pin);
    void tick();

    void open();
    void close();

    bool in_action() const { return in_action_; }
    bool is_opened() const { return is_opened_; }

    void attachOnOpenedEvent(const Slot<bool>& slot)
    {
        onOpened_.attach(slot);
    }

    void attachOnClosedEvent(const Slot<bool>& slot)
    {
        onClosed_.attach(slot);
    }

private:
    bool in_action_;
    bool is_opened_;

    State state_;

    unsigned long time_counter_;

    Signal<bool> onOpened_;
    Signal<bool> onClosed_;

    Servo servo1_;
    Servo servo2_;
};

#endif // ! CAP_HPP