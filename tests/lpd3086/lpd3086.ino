#define ENC_A 2       // пин энкодера
#define ENC_B 3       // пин энкодера
#define ENC_TYPE 1    // тип энкодера, 0 или 1

volatile int encCounter;
volatile boolean state0, lastState, turnFlag;

void setup()
{
  Serial.begin(9600);
  attachInterrupt(0, int0, CHANGE);
}

void int0()
{
  state0 = bitRead(PIND, ENC_A);
  if (state0 != lastState)
  {
    #if (ENC_TYPE == 1)
    turnFlag = !turnFlag;
    if (turnFlag)
    {
        encCounter += (bitRead(PIND, ENC_B) != lastState) ? -1 : 1;
        #else
        encCounter += (bitRead(PIND, ENC_B) != lastState) ? -1 : 1;
        #endif
    }
    lastState = state0;
  }
}

void loop()
{
  Serial.println(encCounter);
  delay(100);
}