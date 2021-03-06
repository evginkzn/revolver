#ifndef PUSHER_HPP
#define PUSHER_HPP

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include <Servo.h>
#include <Callback.h>

class Pusher
{
private:
    enum Angles
    {
        Servo1DefaultAngle = 120,
        Servo2DefaultAngle = 0,
        Servo1UpAngle = 0,
        Servo2PushAngle = 160,
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
    Pusher();
    ~Pusher(){}

public:
    void init(int servo1_pin, int servo2_pin);
    bool in_action() const { return in_action_; }

    void tick();

    void make_push();

    void attachOnPushedEvent(const Slot<bool>& slot)
    {
        onPushed_.attach(slot);
    }

    void set_servo1_default_angle(int angle);
    void set_servo2_default_angle(int angle);
    void set_servo1_action_angle(int angle);
    void set_servo2_action_angle(int angle);

private:
    unsigned long time_counter_;
    bool in_action_;

    State state_;

    Signal<bool> onPushed_;

    Servo servo1_;
    Servo servo2_;

    int servo1_default_angle_;
    int servo2_default_angle_;
    int servo1_action_angle_;
    int servo2_action_angle_;
};

#endif // ! PUSHER_HPP