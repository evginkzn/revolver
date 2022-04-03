#define DRIVER_STEP_TIME 10  // меняем задержку на 10 мкс
#include "GyverStepper2.h"

#define PIN_ENA 4
#define PIN_DIR 5
#define PIN_PUL 11

#define STEP_PER_TURNAROUND 400

#define HOME_POINT_SENSOR_PIN 6
#define CENTER_POINT_SENSOR_PIN 7

typedef enum
{
    REVOLVER_STATE_IDLE = 0,
    REVOLVER_STATE_CALIBRATE,
    REVOLVER_STATE_SELECT_TUBE,
    REVOLVER_STATE_TUBE_CENTRALIZE,
    REVOLVER_STATE_WAIT_HEATER_CAP,
    REVOLVER_STATE_PUSHING,
    REVOLVER_STATE_WAIT_END
} revolver_state_t;

GStepper2<STEPPER2WIRE> step_motor(STEP_PER_TURNAROUND, PIN_PUL, PIN_DIR, PIN_ENA);

static revolver_state_t revolver_state = REVOLVER_STATE_IDLE;

void setup()
{
    pinMode(HOME_POINT_SENSOR_PIN, INPUT);
    pinMode(CENTER_POINT_SENSOR_PIN, INPUT);

    step_motor_configure();

    #ifdef DEBUG
    Serial.begin(9600);
    Serial.println("Initialization done\n");
    #endif // ! DEBUG
}

void loop()
{
    revolver_tick();
}

void step_motor_configure()
{
    step_motor.setMaxSpeed(1200);
    step_motor.setAcceleration(400);
    step_motor.setTarget(800);
    step_motor.invertEn(true);

    #ifdef DEBUG
    Serial.println("Step motor configured");
    #endif // ! DEBUG
}

void revolver_tick()
{
    switch(revolver_state)
    {
        case REVOLVER_STATE_IDLE:
        {

        }
        break;

        case REVOLVER_STATE_CALIBRATE:
        {
            
        }
        break;

        case REVOLVER_STATE_SELECT_TUBE:
        {
            
        }
        break;

        case REVOLVER_STATE_TUBE_CENTRALIZE:
        {
            
        }
        break;

        case REVOLVER_STATE_WAIT_HEATER_CAP:
        {
            
        }
        break;

        case REVOLVER_STATE_PUSHING:
        {
            
        }
        break;

        case REVOLVER_STATE_WAIT_END:
        {
            
        }
        break;
    }
}

bool revolver_find_home()
{
    bool is_found_home = false;

    #ifdef DEBUG
    Serial.println("Start finding home point...");
    #endif // ! DEBUG

    for(int i = 0; i < STEP_PER_TURNAROUND + STEP_PER_TURNAROUND / 2; ++i)
    {
        step_motor.step();

        if (digitalRead(HOME_POINT_SENSOR_PIN) == HIGH)
        {
            is_found_home = true;
            step_motor.pos = 0;
            break;
        }

        delay(10);
    }

    if (is_found_home)
    {
        #ifdef DEBUG
        Serial.println("Home point found.");
        #endif // ! DEBUG
    }
    else
    {
        #ifdef DEBUG
        Serial.println("Home point not found! Error!");
        #endif // ! DEBUG
    }

    return is_found_home;
}