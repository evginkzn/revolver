#define DRIVER_STEP_TIME 10  // меняем задержку на 10 мкс

#include "config.hpp"

#include "GyverStepper2.h"
#include "Callback.h"
#include "ModbusRTUSlave.h"
#include "Button2.h"

#include "revolver.hpp"
#include "pusher.hpp"
#include "cap.hpp"
#include "vendomat.hpp"
#include "service_door.hpp"
#include "modbus_receiver.hpp"

//#define DEBUG

typedef enum
{
    CommandStart = 1
} commands_e;

static const char version[] = "1.1";

GStepper2<STEPPER2WIRE> step_motor(STEP_PER_TURNAROUND, PIN_PUL, PIN_DIR, PIN_ENA);

static Revolver revolver(step_motor, STEP_PER_TURNAROUND
                        , FIRST_TUBE_SENSOR_PIN, TUBE_CENTER_SENSOR_PIN);
static Pusher pusher;
static Cap cap;

static Button2 service_left_button;
static Button2 service_right_button;

static Vendomat vendomat = Vendomat(&revolver, &pusher, &cap, &service_left_button, &service_right_button);
static ServiceDoor service_door = ServiceDoor(SERVICE_DOOR_SENSOR_PIN, false);

static ModbusReceiver modbus_receiver;


bool is_calibrated = false;

static void onCommandRecevedEventHandler(uint16_t command);

void foor(Button2& btn) { Serial.println("foo"); }

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

    //modbus_receiver.init();

    /*while(1)
    {
        modbus_receiver.poll();
    }
    */

    step_motor_configure();

    service_left_button.begin(SERVICE_LEFT_ROTATE_BUTTON);
    service_right_button.begin(SERVICE_RIGHT_ROTATE_BUTTON);

    pusher.init(PUSHER_FIRST_SERVO_PIN, PUSHER_SECOND_SERVO_PIN);
    cap.init(CAP_FIRST_SERVO_PIN, CAP_SECOND_SERVO_PIN);
    revolver.init();
    vendomat.set_mode(Vendomat::ModeMain);
    vendomat.init();

    FunctionSlot<uint16_t> onCommandReceivedSlot(onCommandRecevedEventHandler);
    modbus_receiver.attachOnCommandReceivedEvent(onCommandReceivedSlot);

    FunctionSlot<bool> onServiceDoorOpenedSlot(onServiceDoorOpenedHandler);
    service_door.attachOnOpenedEvent(onServiceDoorOpenedSlot);

    FunctionSlot<bool> onServiceDoorClosedSlot(onServiceDoorClosedHandler);
    service_door.attachOnClosedEvent(onServiceDoorClosedSlot);

    #ifdef DEBUG
    Serial.println("Initialization done\n");
    #endif
}

void loop()
{
    while ( ! is_calibrated)
    {
        is_calibrated = revolver.find_first_tube();
    }

    //modbus_receiver.poll();

    vendomat.tick();
    revolver.tick();
    step_motor.tick();
    pusher.tick();
    cap.tick();
    service_door.tick();    

    //modbus_receiver.updateStateHandler(vendomat.stage());

    service_left_button.loop();
    service_right_button.loop();
}

void step_motor_configure()
{
    step_motor.setMaxSpeed(10000);
    step_motor.setAcceleration(4000);
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

static void onServiceDoorOpenedHandler(bool)
{
    uint32_t timeout_counter;
    while(vendomat.stage() != VendomatModeMain::Stage::StageStandBy)
    {
        ++timeout_counter;
        if (timeout_counter >= 0xFFFFFF)
        {
            break;
        }
    }
    vendomat.set_mode(Vendomat::ModeService);
    #ifdef DEBUG
    Serial.println("Service door opened");
    #endif // ! DEBUG
}

static void onServiceDoorClosedHandler(bool)
{
    vendomat.set_mode(Vendomat::ModeMain);
    is_calibrated = false;
    #ifdef DEBUG
    Serial.println("Service door closed");
    #endif // ! DEBUG
}