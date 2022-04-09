#include "pusher.hpp"

#define DEBUG

Pusher::Pusher()
    : in_action_(false)
    , state_(StateIdle)
{

}

void Pusher::init(int servo1_pin, int servo2_pin)
{   
    servo1_.attach(servo1_pin);
    servo2_.attach(servo2_pin);

    servo1_.write(120);
    servo2_.write(120);

    #ifdef DEBUG
    Serial.println("Pusher initialized");
    #endif // ! DEBUG
}

void Pusher::tick()
{
    switch (state_)
    {
        case StateIdle:
        {
            
        }
        break;

        case StateFirstServoAction:
        {
            #ifdef DEBUG
            Serial.println("First servo action");
            #endif // ! DEBUG
            servo1_.write(120);
            time_counter_ = millis();
            state_ = StateSecondServoAction;
        }
        break;

        case StateSecondServoAction:
        {
            if (millis() - time_counter_ > 500)
            {
                #ifdef DEBUG
                Serial.println("Second servo action");
                #endif // ! DEBUG
                servo2_.write(0);
                time_counter_ = millis();
                state_ = StateSecondServoBack;
            }
        }
        break;

        case StateSecondServoBack:
        {
            if (millis() - time_counter_ > 600)
            {
                #ifdef DEBUG
                Serial.println("Second servo back");
                #endif // ! DEBUG
                servo2_.write(120);
                time_counter_ = millis();
                state_ = StateFirstServoBack;
            }
        }
        break;

        case StateFirstServoBack:
        {
            if (millis() - time_counter_ > 700)
            {
                #ifdef DEBUG
                Serial.println("First servo back");
                #endif // ! DEBUG
                servo1_.write(0);
                time_counter_ = millis();
                state_ = StateIdle;
                in_action_ = false;
                #ifdef DEBUG
                Serial.println("Pushed");
                #endif // ! DEBUG
            }
        }
        break;
    
        default:
            break;
    }
}

void Pusher::make_push()
{
    state_ = StateFirstServoAction;
    in_action_ = true;
    #ifdef DEBUG
    Serial.println("Make push...");
    #endif // ! DEBUG
}