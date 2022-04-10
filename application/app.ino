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

#define FIRST_TUBE_SENSOR_PIN 8
#define TUBE_CENTER_SENSOR_PIN 9

#define PUSHER_FIRST_SERVO_PIN 2
#define PUSHER_SECOND_SERVO_PIN 3

#define CAP_FIRST_SERVO_PIN 6
#define CAP_SECOND_SERVO_PIN 7

static const char version[] = "0.4";

GStepper2<STEPPER2WIRE> step_motor(STEP_PER_TURNAROUND, PIN_PUL, PIN_DIR, PIN_ENA);

static Revolver revolver(step_motor, STEP_PER_TURNAROUND
                        , FIRST_TUBE_SENSOR_PIN, TUBE_CENTER_SENSOR_PIN);
static Pusher pusher;
static Cap cap;

static uint8_t tube_target_ = 0;
static uint8_t current_tube_ = 0;

static bool is_pushed_ = true;

void setup()
{
    #ifdef DEBUG
    Serial.begin(9600);
    #endif // ! DEBUG

    #ifdef DEBUG
    Serial.print("Firmware version: ");
    Serial.print(version);
    Serial.println("");
    #endif // ! DEBUG

    step_motor_configure();

    pusher.init(PUSHER_FIRST_SERVO_PIN, PUSHER_SECOND_SERVO_PIN);
    cap.init(CAP_FIRST_SERVO_PIN, CAP_SECOND_SERVO_PIN);
    revolver.init();

    FunctionSlot<uint8_t> selected_event_slot(tube_selected_event);
    revolver.attachOnSelectEvent(selected_event_slot);

    FunctionSlot<bool> pushed_event_slot(pushed_event);

    #ifdef DEBUG
    Serial.println("Initialization done\n");
    #endif // ! DEBUG

}

uint8_t tubes_selecting_test_targets[12] = { 1,  2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0 };
unsigned long select_period_counter = 0;
uint8_t selector = 0;

bool is_calibrated = false;

void loop()
{
    while ( ! is_calibrated)
    {
        is_calibrated = revolver.find_first_tube();

    }

    if (is_pushed_ && ! pusher.in_action())
    {
        if (millis() - select_period_counter > 5000)
        {
            select_period_counter = millis();
            if (revolver.state() == Revolver::StateIdle)
            {
                is_pushed_ = false;
                revolver.select_tube(tubes_selecting_test_targets[selector]);

                ++selector;
                if (selector >= 12)
                {
                    selector = 0;
                }
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
    step_motor.setMaxSpeed(12000);
    step_motor.setAcceleration(9000);
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

    cap.open();

    pusher.make_push();
}

void pushed_event(bool)
{
    is_pushed_ = true;
    cap.close();
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