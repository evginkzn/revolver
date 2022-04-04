#define DRIVER_STEP_TIME 10  // меняем задержку на 10 мкс
#include "GyverStepper2.h"

#define PIN_ENA 4
#define PIN_DIR 5
#define PIN_PUL 11

#define STEP_PER_TURNAROUND 400

GStepper2<STEPPER2WIRE> stepper(STEP_PER_TURNAROUND, PIN_PUL, PIN_DIR, PIN_ENA);

void setup()
{
    stepper.setMaxSpeed(1200);     // скорость движения к цели
    stepper.setAcceleration(400); // ускорение

    stepper.invertEn(true);
    stepper.enable();

    Serial.begin(9600);
    Serial.print("Initialization done\r\n");
}

bool dir = 1;

void loop()
{
    for(int i = 0; i < STEP_PER_TURNAROUND; ++i)
    {
        stepper.step();
        delay(10);
    }

    if (stepper.dir == 1)
    {
        stepper.dir = -1;
    }
    else
    {
        stepper.dir = 1;
    }

    delay(2000);
}