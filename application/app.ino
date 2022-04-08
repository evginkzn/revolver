#define DRIVER_STEP_TIME 10  // меняем задержку на 10 мкс
#define DEBUG

#include "GyverStepper2.h"
#include "Callback.h"

#include "revolver.hpp"
#include "pusher.hpp"
#include "cap.hpp"

#define PIN_ENA 4
#define PIN_DIR 5
#define PIN_PUL 11

#define STEP_PER_TURNAROUND 3200

#define FIRST_TUBE_SENSOR_PIN 6
#define TUBE_CENTER_SENSOR_PIN 7


GStepper2<STEPPER2WIRE> step_motor(STEP_PER_TURNAROUND, PIN_PUL, PIN_DIR, PIN_ENA);

static Revolver revolver(step_motor, STEP_PER_TURNAROUND
                        , FIRST_TUBE_SENSOR_PIN, TUBE_CENTER_SENSOR_PIN);
static Pusher pusher;
static Cap cap;

static uint8_t tube_target_ = 0;
static uint8_t current_tube_ = 0;

void setup()
{
    #ifdef DEBUG
    Serial.begin(9600);
    #endif // ! DEBUG

    step_motor_configure();

    revolver.init();
    pusher.init();
    cap.init();

    FunctionSlot<uint8_t> selected_event_slot(tube_selected_event);
    revolver.attachOnSelectEvent(selected_event_slot);

    #ifdef DEBUG
    Serial.println("Initialization done\n");
    #endif // ! DEBUG

}

uint8_t tubes_selecting_test_targets[10] = { 3, 11, 14, 2, 8, 0, 6, 2, 11, 0 };
unsigned long select_period_counter = 0;
uint8_t selector = 0;

void loop()
{
    if (millis() - select_period_counter > 15000)
    {
        select_period_counter = millis();
        if (revolver.state() == Revolver::StateIdle)
        {
            revolver.select_tube(tubes_selecting_test_targets[selector]);

            ++selector;
            if (selector >= 10)
            {
                selector = 0;
            }
        }
    }

    revolver.tick();
    step_motor.tick();
    pusher.tick();
    cap.tick();
}

void step_motor_configure()
{
    step_motor.setMaxSpeed(6400);
    step_motor.setAcceleration(6400);
    step_motor.invertEn(true);

    #ifdef DEBUG
    Serial.println("Step motor configured");
    #endif // ! DEBUG
}

void tube_selected_event(uint8_t selected_tube)
{
    #ifdef DEBUG
    Serial.print("Tube ");
    Serial.print(selected_tube);
    Serial.println(" selected");
    #endif // ! DEBUG

    pusher.make_push();
}

void cap_open()
{
    #ifdef DEBUG
    Serial.println("Cap opened");
    #endif // ! DEBUG
}

void cap_close()
{
    #ifdef DEBUG
    Serial.println("Cap closed");
    #endif // ! DEBUG
}