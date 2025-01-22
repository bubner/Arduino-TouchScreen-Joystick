#include <Adafruit_GFX.h>
#include <Duino.h>

#define CURSOR_ANGLE 4
#define CURSOR_LENGTH 15
#define CURSOR_INSET 7
#define UPDATE_INTERVAL_MS 150 // reduces flicker

Arduino *arduino;

void setup()
{
    arduino = new Arduino(JOYSTICK_ACCUMULATING);
    arduino->screen.fillScreen(GREEN);
    arduino->screen.fillRect(0, 0, arduino->screen.width(), arduino->screen.height() / 3, RED);
    arduino->screen.fillRect(0, arduino->screen.height() / 3, arduino->screen.width(), arduino->screen.height() / 3, BLUE);
}

void loop()
{
    static Point prev_point;
    static uint16_t prev_colours[CURSOR_LENGTH + 4][CURSOR_LENGTH + 4]; // colours under cursor
    static unsigned long prev_update = 0;

    unsigned long current_update = millis();
    if (current_update - prev_update >= UPDATE_INTERVAL_MS)
    {
        Point p = arduino->getPoint();
        if (!p.invalid) // is moving
        {
            // restore the previous cursor position with the stored colours
            if (!prev_point.invalid)
            {
                for (int i = 0; i < CURSOR_LENGTH + 4; i++)
                {
                    for (int j = 0; j < CURSOR_LENGTH + 4; j++)
                    {
                        int x = prev_point.x - 2 + i;
                        int y = prev_point.y - 2 + j;
                        if (x >= 0 && x < arduino->screen.width() && y >= 0 && y < arduino->screen.height())
                        {
                            arduino->screen.drawPixel(x, y, prev_colours[i][j]);
                        }
                    }
                }
            }

            // store the current colours under the new cursor position
            for (int i = 0; i < CURSOR_LENGTH + 4; i++)
            {
                for (int j = 0; j < CURSOR_LENGTH + 4; j++)
                {
                    int x = p.x - 2 + i;
                    int y = p.y - 2 + j;
                    if (x >= 0 && x < arduino->screen.width() && y >= 0 && y < arduino->screen.height())
                    {
                        prev_colours[i][j] = arduino->screen.readPixel(x, y); // store by switching to read mode and back
                    }
                }
            }

            // draw cursor
            arduino->screen.fillTriangle(p.x, p.y, p.x + CURSOR_ANGLE, p.y + CURSOR_LENGTH, p.x + CURSOR_INSET, p.y + CURSOR_INSET, WHITE);
            arduino->screen.fillTriangle(p.x, p.y, p.x + CURSOR_LENGTH, p.y + CURSOR_ANGLE, p.x + CURSOR_INSET, p.y + CURSOR_INSET, WHITE);

            prev_point = p;
        }

        prev_update = current_update;
    }
}