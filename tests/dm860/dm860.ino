#define PIN_ENA 4
#define PIN_DIR 5
#define PIN_PUL 10

void setup()
{
    pinMode(PIN_DIR, OUTPUT);
    pinMode(PIN_ENA, OUTPUT);

    Serial.begin(9600);

    Serial.print("Initialization done\r\n");
}

void loop()
{
    
}