#define DRIVER_STEP_TIME 10  // меняем задержку на 10 мкс
#define DEBUG

#include "GyverStepper2.h"
#include "Callback.h"

#include "revolver.hpp"
#include "pusher.hpp"
#include "cap.hpp"
#include "vendomat.hpp"

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

static const char version[] = "0.5";

GStepper2<STEPPER2WIRE> step_motor(STEP_PER_TURNAROUND, PIN_PUL, PIN_DIR, PIN_ENA);

static Revolver revolver(step_motor, STEP_PER_TURNAROUND
                        , FIRST_TUBE_SENSOR_PIN, TUBE_CENTER_SENSOR_PIN);
static Pusher pusher;
static Cap cap;

static Vendomat vendomat = Vendomat(revolver, pusher, cap);

static char command_buffer[128];
static uint8_t command_buffer_offset = 0;
static unsigned long command_rx_timeout = 0;

bool is_calibrated = false;

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
    vendomat.init();

    #ifdef DEBUG
    Serial.println("Initialization done\n");
    #endif // ! DEBUG

}

void loop()
{
    while ( ! is_calibrated)
    {
        is_calibrated = revolver.find_first_tube();
    }

    uint32_t bytes_available = Serial.available();
    if (bytes_available > 0)
    {
        Serial.readBytes(command_buffer + command_buffer_offset, bytes_available);
        command_buffer_offset += bytes_available;
        command_rx_timeout = millis();
    }

    if ((millis() - command_rx_timeout) > 1000 && command_buffer_offset)
    {
        if (strstr(command_buffer, "set tube"))
        {
            int tube = atoi(command_buffer + 9);
            Serial.println("Found command SET_TUBE");
            Serial.print("Tube: ");
            Serial.print(tube);
            Serial.println("");

            vendomat.select_cell(tube);
        }
        memset(command_buffer, 0, sizeof(command_buffer));
        command_buffer_offset = 0;
    }

    vendomat.tick();
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