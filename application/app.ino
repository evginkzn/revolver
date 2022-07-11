#define DRIVER_STEP_TIME 10  // меняем задержку на 10 мкс
//#define DEBUG

#include "GyverStepper2.h"
#include "Callback.h"
#include "ModbusRTUSlave.h"

#include "revolver.hpp"
#include "pusher.hpp"
#include "cap.hpp"
#include "vendomat.hpp"
#include "modbus_receiver.hpp"

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

typedef enum
{
    CommandStart = 1
} commands_e;

static const char version[] = "1.0";

GStepper2<STEPPER2WIRE> step_motor(STEP_PER_TURNAROUND, PIN_PUL, PIN_DIR, PIN_ENA);

static Revolver revolver(step_motor, STEP_PER_TURNAROUND
                        , FIRST_TUBE_SENSOR_PIN, TUBE_CENTER_SENSOR_PIN);
static Pusher pusher;
static Cap cap;

static Vendomat vendomat = Vendomat(&revolver, &pusher, &cap);

static ModbusReceiver modbus_receiver;

bool is_calibrated = false;

static void onCommandRecevedEventHandler(uint16_t command);

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

    modbus_receiver.init();

    /*while(1)
    {
        modbus_receiver.poll();
    }
    */

    step_motor_configure();

    pusher.init(PUSHER_FIRST_SERVO_PIN, PUSHER_SECOND_SERVO_PIN);
    cap.init(CAP_FIRST_SERVO_PIN, CAP_SECOND_SERVO_PIN);
    revolver.init();
    vendomat.init();

    FunctionSlot<uint16_t> onCommandReceivedSlot(onCommandRecevedEventHandler);
    modbus_receiver.attachOnCommandReceivedEvent(onCommandReceivedSlot);

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

    modbus_receiver.poll();

    vendomat.tick();
    revolver.tick();
    step_motor.tick();
    pusher.tick();
    cap.tick();

    modbus_receiver.updateStateHandler(vendomat.stage());
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

static void onCommandRecevedEventHandler(uint16_t command)
{
    if (command == CommandStart)
    {
        if (vendomat.stage() == VendomatModeMain::Stage::StageStandBy)
        {
            vendomat.select_cell(modbus_receiver.selectedTube());
        }
    }
}