#ifndef REVOLVER_HPP
#define REVOLVER_HPP

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include "GyverStepper2.h"
#include <Callback.h>

class Revolver
{
public:
    enum State
    {
        StateIdle = 0,
        StateCalibrating,
        StateSelectingTube,
    };

public:
    Revolver(GStepper2<STEPPER2WIRE>& motor, uint16_t steps
            , int first_tube_sensor_pin, int center_sensor_pin);
    ~Revolver(){}

public:
    void init();
    void tick();

    bool select_tube(uint8_t tube);

    State state() const { return state_; }

    void attachOnSelectEvent(const Slot<uint8_t>& slot)
    {
        onSelect_.attach(slot);
    }

    bool find_first_tube();

private:
    bool find_tube_center();

private:
    GStepper2<STEPPER2WIRE>& motor_;

    State state_;
    uint16_t steps_;

    int first_tube_sensor_pin_;
    int center_sensor_pin_;

    float current_deg_ = 0;
    uint8_t current_tube_ = 0;

    bool is_reversed_;

    Signal<uint8_t> onSelect_;
};

#endif // ! REVOLVER_HPP