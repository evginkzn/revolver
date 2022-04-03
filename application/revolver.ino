#define DRIVER_STEP_TIME 10  // меняем задержку на 10 мкс
#include "GyverStepper2.h"

#define PIN_ENA 4
#define PIN_DIR 5
#define PIN_PUL 11

#define STEP_PER_TURNAROUND 400

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
    step_motor_configure();

    #ifdef DEBUG
    Serial.begin(9600);
    Serial.print("Initialization done\r\n");
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

void revolver_calibrate()
{
    for(int i = 0; i < STEP_PER_TURNAROUND + STEP_PER_TURNAROUND / 2; ++i)
    {
        step_motor.step();

        

        delay(10);
    }
}