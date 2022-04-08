#include "revolver.hpp"

#define DEBUG

Revolver::Revolver(GStepper2<STEPPER2WIRE>& motor, uint16_t steps
            , uint8_t first_tube_sensor_pin, uint8_t center_sensor_pin)
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

    //find_first_tube();
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

    for(int i = 0; i < steps_ + (steps_ / 2); ++i)
    {
        motor_.step();

        if (digitalRead(first_tube_sensor_pin_) == HIGH)
        {
            find_tube_center();

            is_found_home = true;
            motor_.pos = 0;
            break;
        }

        //delay(1);
    }

    motor_.disable();

    if (is_found_home)
    {
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

    for(int i = 0; i < 10; ++i)
    {
        if (digitalRead(center_sensor_pin_) == HIGH)
        {
            is_found_center = true;
            break;
        }

        motor_.step();

        delay(1);
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