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

void loop()
{
    long ms = millis();
    arduino->draw_function([&ms](double x) -> double
                           { return sin(x + ms / 1E9); }, RED);
    arduino->draw_function([&ms](double x) -> double
                           { return cos(x + ms / 1E9); }, CYAN);
    arduino->reset_screen();
}
