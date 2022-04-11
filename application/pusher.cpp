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

    servo1_.write(Servo1DefaultAngle); // подниматель
    servo2_.write(Servo2DefaultAngle); // нижний

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
            servo1_.write(Servo1UpAngle);
            time_counter_ = millis();
            state_ = StateSecondServoAction;
        }
        break;

        case StateSecondServoAction:
        {
            if (millis() - time_counter_ > 1500)
            {
                #ifdef DEBUG
                Serial.println("Second servo action");
                #endif // ! DEBUG
                servo2_.write(Servo2PushAngle);
                time_counter_ = millis();
                state_ = StateSecondServoBack;
            }
        }
        break;

        case StateSecondServoBack:
        {
            if (millis() - time_counter_ > 1600)
            {
                #ifdef DEBUG
                Serial.println("Second servo back");
                #endif // ! DEBUG
                servo2_.write(Servo2DefaultAngle);
                time_counter_ = millis();
                state_ = StateFirstServoBack;
            }
        }
        break;

        case StateFirstServoBack:
        {
            if (millis() - time_counter_ > 1700)
            {
                #ifdef DEBUG
                Serial.println("First servo back");
                #endif // ! DEBUG
                servo1_.write(Servo1DefaultAngle);
                time_counter_ = millis();
                state_ = StateIdle;
                in_action_ = false;
                #ifdef DEBUG
                Serial.println("Pushed");
                onPushed_.fire(true);
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