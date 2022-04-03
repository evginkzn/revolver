#define DRIVER_STEP_TIME 10  // меняем задержку на 10 мкс
#include "GyverStepper2.h"

#define PIN_ENA 4
#define PIN_DIR 5
#define PIN_PUL 11

GStepper2<STEPPER2WIRE> stepper(400, PIN_PUL, PIN_DIR, PIN_ENA);

void setup()
{
    stepper.setMaxSpeed(1200);     // скорость движения к цели
    stepper.setAcceleration(400); // ускорение
    stepper.setTarget(800);       // цель

    stepper.invertEn(true);
    stepper.enable();

    Serial.begin(9600);
    Serial.print("Initialization done\r\n");
}

bool dir = 1;

void loop()
{
    /*stepper.tick();   // мотор асинхронно крутится тут
    // если приехали
    if (stepper.ready())
    {
        dir = !dir;   // разворачиваем
        stepper.setTarget(dir * 800); // едем в другую сторону
    }*/

    for(int i = 0; i < 400; ++i)
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