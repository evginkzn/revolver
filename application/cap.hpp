#ifndef CAP_HPP
#define CAP_HPP

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include <Callback.h>

class Cap
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
    Cap();
    ~Cap(){}

public:
    void init();
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
};

#endif // ! CAP_HPP