#include <Adafruit_GFX.h>
#include <Duino.h>

Arduino *arduino;

void setup()
{
    arduino = new Arduino(NO_INPUT);
    arduino->reset_screen();
}

void loop()
{
}
