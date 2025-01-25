#include <Adafruit_GFX.h>
#include <Duino.h>

#define CURSOR_ANGLE 4
#define CURSOR_LENGTH 15
#define CURSOR_REDRAW_PADDING 2
#define CURSOR_INSET 7
#define UPDATE_INTERVAL_MS 100 // reduces flicker

Arduino *arduino;

void setup()
{
    arduino = new Arduino(JOYSTICK_ACCUMULATING);
    arduino->screen.fillScreen(GREEN);
    arduino->screen.fillRect(0, 0, arduino->screen.width(), arduino->screen.height() / 3, RED);
    arduino->screen.fillRect(0, arduino->screen.height() / 3, arduino->screen.width(), arduino->screen.height() / 3, BLUE);
    // arduino->set_joystick_accumulation_origin(SCREEN_MIDDLE_X, SCREEN_MIDDLE_Y);
}

void loop()
{
    static Point *prev_point = nullptr;
    static uint16_t prev_colours[CURSOR_LENGTH + CURSOR_REDRAW_PADDING][CURSOR_LENGTH + CURSOR_REDRAW_PADDING]; // colours under cursor
    static unsigned long prev_update = 0;

    unsigned long current_update = millis();
    if (current_update - prev_update >= UPDATE_INTERVAL_MS)
    {
        Point p = arduino->getPoint();
        // allow resetting of the origin via touch
        arduino->pointMode = TOUCH_SCREEN;
        Point tp = arduino->getPoint();
        if (!tp.invalid)
        {
            arduino->set_joystick_accumulation_origin(tp.x, tp.y);
            p = tp; // use touch point for this render
        }
        arduino->pointMode = JOYSTICK_ACCUMULATING;
        if (!p.invalid) // is moving
        {
            // restore the previous cursor position with the stored colours
            if (prev_point != nullptr)
            {
                for (int i = 0; i < CURSOR_LENGTH + CURSOR_REDRAW_PADDING; i++)
                {
                    for (int j = 0; j < CURSOR_LENGTH + CURSOR_REDRAW_PADDING; j++)
                    {
                        int x = prev_point->x + i; // TODO: 0,0 does have some strange behaviours
                        int y = prev_point->y + j;
                        if (x >= 0 && x < arduino->screen.width() && y >= 0 && y < arduino->screen.height())
                        {
                            arduino->screen.drawPixel(x, y, prev_colours[i][j]);
                        }
                    }
                }
            }

            // store the current colours under the new cursor position
            for (int i = 0; i < CURSOR_LENGTH + CURSOR_REDRAW_PADDING; i++)
            {
                for (int j = 0; j < CURSOR_LENGTH + CURSOR_REDRAW_PADDING; j++)
                {
                    int x = p.x + i;
                    int y = p.y + j;
                    if (x >= 0 && x < arduino->screen.width() && y >= 0 && y < arduino->screen.height())
                    {
                        prev_colours[i][j] = arduino->screen.readPixel(x, y); // store by switching to read mode and back
                    }
                }
            }

            delete prev_point;
            prev_point = new Point(p);
        }

        if (prev_point != nullptr)
        {
            arduino->screen.fillTriangle(p.x, p.y, p.x + CURSOR_ANGLE, p.y + CURSOR_LENGTH, p.x + CURSOR_INSET, p.y + CURSOR_INSET, p.z ? BLACK : WHITE);
            arduino->screen.fillTriangle(p.x, p.y, p.x + CURSOR_LENGTH, p.y + CURSOR_ANGLE, p.x + CURSOR_INSET, p.y + CURSOR_INSET, p.z ? BLACK : WHITE);
        }

        prev_update = current_update;
    }
}