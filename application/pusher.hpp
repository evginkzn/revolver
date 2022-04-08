#ifndef PUSHER_HPP
#define PUSHER_HPP

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include <Callback.h>

class Pusher
{
private:
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
    void init();
    bool in_action() const { return in_action_; }

    void tick();

    void make_push();

    void attachOnPushedEvent(const Slot<bool>& slot)
    {
        onPushed_.attach(slot);
    }


private:
    unsigned long time_counter_;
    bool in_action_;

    State state_;

    Signal<bool> onPushed_;
};

#endif // ! PUSHER_HPP