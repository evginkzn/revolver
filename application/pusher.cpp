#include "pusher.hpp"

Pusher::Pusher()
    : in_action_(false)
    , state_(StateIdle)
    , servo1_default_angle_(Servo1DefaultAngle)
    , servo2_default_angle_(Servo2DefaultAngle)
    , servo1_action_angle_(Servo1UpAngle)
    , servo2_action_angle_(Servo2PushAngle)
{

}

void Pusher::init(int servo1_pin, int servo2_pin)
{   
    servo1_.attach(servo1_pin);
    servo2_.attach(servo2_pin);

    servo1_.write(servo1_default_angle_); // подниматель
    servo2_.write(servo2_default_angle_); // нижний

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
            servo1_.write(servo1_action_angle_);
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
                servo2_.write(servo2_action_angle_);
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
                servo2_.write(servo2_default_angle_);
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
                servo1_.write(servo1_default_angle_);
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

void Pusher::set_servo1_default_angle(int angle)
{
    if (angle >= 0 && angle <= 180)
    {
        servo1_default_angle_ = angle;
    }
}

void Pusher::set_servo2_default_angle(int angle)
{
    if (angle >= 0 && angle <= 180)
    {
        servo2_default_angle_ = angle;
    }
}

void Pusher::set_servo1_action_angle(int angle)
{
    if (angle >= 0 && angle <= 180)
    {
        servo1_action_angle_ = angle;
    }
}

void Pusher::set_servo2_action_angle(int angle)
{
    if (angle >= 0 && angle <= 180)
    {
        servo2_action_angle_ = angle;
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