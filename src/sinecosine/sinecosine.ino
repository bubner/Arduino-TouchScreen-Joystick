#include <Adafruit_GFX.h>
#include <Duino.h>

Arduino *arduino;

void setup()
{
    arduino = new Arduino(NO_INPUT);
    arduino->reset_screen();
    DF_LX = -10;
    DF_UX = 10;
    DF_LY = -1;
    DF_UY = 1;
}

double f(double x)
{
    return sin(x + millis() / 1E9);
}

double g(double x)
{
    return cos(x + millis() / 1E9);
}

void loop()
{
    arduino->draw_function(f, RED);
    arduino->draw_function(g, CYAN);
    arduino->reset_screen();
}
