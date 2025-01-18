#include <Adafruit_GFX.h>
#include "util.h"
#include "config.h"
#include "geometry.h"
#include "api.h"

Arduino *arduino;

void setup()
{
    arduino = new Arduino(NO_INPUT);
    arduino->reset_screen();
}

void loop()
{
}
