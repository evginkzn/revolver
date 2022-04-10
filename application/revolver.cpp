#include "revolver.hpp"

#define DEBUG

Revolver::Revolver(GStepper2<STEPPER2WIRE>& motor, uint16_t steps
            , int first_tube_sensor_pin, int center_sensor_pin)
    : motor_(motor)
    , steps_(steps)
    , first_tube_sensor_pin_(first_tube_sensor_pin)
    , center_sensor_pin_(center_sensor_pin)
    , state_(StateIdle)
{
}

void Revolver::init()
{
    pinMode(first_tube_sensor_pin_, INPUT);
    pinMode(center_sensor_pin_, INPUT);
}

void Revolver::tick()
{
    switch(state_)
    {
        case StateIdle:
        {
            //motor_.enable();
            //motor_.setTargetDeg((double)tube_target_ * 30.0);
            //state_ = StateSelectingTube;
        }
        break;

        case StateCalibrating:
        {
            
        }
        break;

        case StateSelectingTube:
        {
            if (motor_.ready())
            {
                find_tube_center();
                motor_.disable();
                state_ = StateIdle;
                onSelect_.fire(current_tube_);
            }
        }
        break;

        default: break;
    }
}

void Revolver::select_tube(uint8_t tube)
{
    if (current_tube_ == tube || tube > 11)
    {
        return;
    }

    #ifdef DEBUG
    Serial.println("_________________________");
    Serial.println("Stelect tube: ");
    Serial.println(tube);
    Serial.println("Current tube: ");
    Serial.println(current_tube_);
    Serial.println("Current deg: ");
    Serial.println(current_deg_);
    #endif // ! DEBUG    

    float target_deg = (float)tube * 30.0;
    float deg_diff = target_deg - current_deg_;

    bool need_change_direction = false;
    float delta_deg = 0;
    int32_t need_steps = 0;

    float path1 = target_deg - current_deg_;

    #ifdef DEBUG
    Serial.println("Path #1: ");
    Serial.println(path1);
    #endif // ! DEBUG

    float path2 = 0;
    if (target_deg >= current_deg_)
    {
        path2 = (360.0 - target_deg) + current_deg_;
    }
    else
    {
        path2 = (360.0 - current_deg_) + target_deg;
    }

    #ifdef DEBUG
    Serial.println("Path #2: ");
    Serial.println(path2);
    #endif // ! DEBUG

    delta_deg = (fabs(path1) > fabs(path2)) ? fabs(path2) : fabs(path1);

    #ifdef DEBUG
    Serial.println("So, shortest path: ");
    Serial.println(delta_deg);
    #endif // ! DEBUG

    need_steps = (int32_t)(delta_deg / (360.0 / (float)steps_));
    need_steps *= 50;
    
    if (fabs(path2) < fabs(path1))
    {
        if (path1 > 0)
        {
            need_steps = -need_steps;
        }
    }
    else
    {
        if (path1 < 0)
        {
            need_steps = -need_steps;
        }
    }

    #ifdef DEBUG
    Serial.println("Need steps: ");
    Serial.println(need_steps, 10);
    Serial.println("____________________________");
    #endif // ! DEBUG

    current_tube_ = tube;
    current_deg_ = target_deg;

    motor_.pos = 0;
    motor_.enable();
    motor_.setTarget(need_steps);

    state_ = StateSelectingTube;
}

bool Revolver::find_first_tube()
{
    bool is_found_home = false;

    #ifdef DEBUG
    Serial.println("Start finding first tube...");
    #endif // ! DEBUG

    motor_.enable();

    while(1)
    {
        motor_.setSpeed(6400);
        motor_.tick();

        if (digitalRead(first_tube_sensor_pin_) == HIGH)
        {
            is_found_home = true;
            break;
        }
    }

    motor_.disable();

    if (is_found_home)
    {
        find_tube_center();
        #ifdef DEBUG
        Serial.println("First tube found.");
        #endif // ! DEBUG
    }
    else
    {
        #ifdef DEBUG
        Serial.println("First tube not found! Error!");
        #endif // ! DEBUG
    }

    return is_found_home;
}

bool Revolver::find_tube_center()
{
    bool is_found_center = false;

    #ifdef DEBUG
    Serial.println("Start finding tube center...");
    #endif // ! DEBUG

    motor_.enable();

    for(int i = 0; i < 2200; ++i)
    {
        motor_.step();
        motor_.tick();

        if (digitalRead(center_sensor_pin_) == LOW)
        {
            is_found_center = true;
            break;
        }

        delay(1);
    }

    if ( ! is_found_center)
    {
        motor_.dir *= -1;
        
        for(int i = 0; i < 4400; ++i)
        {
            motor_.step();
            motor_.tick();
            if (digitalRead(center_sensor_pin_) == LOW)
            {
            is_found_center = true;
            break;
            }

            delay(1);
        }

        motor_.dir *= -1;
    }

    if ( ! is_found_center)
    {   
        for(int i = 0; i < 2200; ++i)
        {
            motor_.step();
            motor_.tick();
            if (digitalRead(center_sensor_pin_) == LOW)
            {
            is_found_center = true;
            break;
            }

            delay(1);
        }
    }

    motor_.disable();

    if (is_found_center)
    {
        #ifdef DEBUG
        Serial.println("Tube center found.");
        #endif // ! DEBUG
        
        return true;
    }
    else
    {
        #ifdef DEBUG
        Serial.println("Tube center point not found! Error!");
        #endif // ! DEBUG
        
        return false;
    }
}