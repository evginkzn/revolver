#include "cap.hpp"

Cap::Cap()
    : in_action_(false)
    , is_opened_(false)
    , state_(StateIdle)
{

}

void Cap::init()
{
    #ifdef DEBUG
    Serial.println("Cap initialized");
    #endif // ! DEBUG
}

void Cap::tick()
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
            Serial.println("Cap::First servo action");
            #endif // ! DEBUG
            //servo1.write(deg);
            time_counter_ = millis();
            state_ = StateSecondServoAction;
        }
        break;

        case StateSecondServoAction:
        {
            if (millis() - time_counter_ > 500)
            {
                #ifdef DEBUG
                Serial.println("Cap::Second servo action");
                Serial.println("Cap:: opened");
                #endif // ! DEBUG
                //servo2.write(deg);
                time_counter_ = millis();
                state_ = StateIdle;
                is_opened_ = true;
                in_action_ = false;
            }
        }
        break;

        case StateSecondServoBack:
        {
            #ifdef DEBUG
            Serial.println("Cap::Second servo back");
            #endif // ! DEBUG
            //servo2.write(deg);
            time_counter_ = millis();
            state_ = StateFirstServoBack;
        }
        break;

        case StateFirstServoBack:
        {
            if (millis() - time_counter_ > 700)
            {
                #ifdef DEBUG
                Serial.println("Cap::First servo back");
                #endif // ! DEBUG
                //servo1.write(deg);
                time_counter_ = millis();
                state_ = StateIdle;
                in_action_ = false;
                is_opened_ = false;
                #ifdef DEBUG
                Serial.println("Cap:: closed");
                #endif // ! DEBUG
            }
        }
        break;
    
        default:
            break;
    }
}

void Cap::open()
{
    if (in_action_)
    {
        return;
    }

    #ifdef DEBUG
    Serial.println("Cap:: opening...");
    #endif // ! DEBUG
    
    in_action_ = true;
    state_ = StateFirstServoAction;
}

void Cap::close()
{
    if (in_action_)
    {
        return;
    }

    #ifdef DEBUG
    Serial.println("Cap:: closing...");
    #endif // ! DEBUG

    in_action_ = true;
    state_ = StateSecondServoBack;
}