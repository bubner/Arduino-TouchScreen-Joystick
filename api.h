#pragma once
#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>
#include "config.h"
#include "geometry.h"
#include "util.h"

// default bounds for draw_function
#define LX -100
#define UX 100
#define LY -100
#define UY 100
#define DT 0.01

enum PointMode
{
    NO_INPUT,
    TOUCH_SCREEN,
    JOYSTICK_ABSOLUTE,
    JOYSTICK_ACCUMULATING
};

class Arduino
{
public:
    Adafruit_TFTLCD screen;
    TouchScreen touch;
    /**
     * type of point data returned from getPoint()
     */
    PointMode pointMode;
    /**
     * current point as per the current PointMode
     */
    Point getPoint();
    /**
     * draw an unary function e.g. `arduino->draw_function([](double x) -> double { return x; }, WHITE);` will draw a white y=x function with default bounds
     */
    template <typename T>
    void draw_function(T func, uint16_t color, int lower_x = LX, int upper_x = UX, int lower_y = LY, int upper_y = UY, float dt = DT);
    Arduino(PointMode pointMode);
    /**
     * shorthand for `arduino->screen.fillScreen(rgb(0, 0, 0));`
     */
    void reset_screen();
};

/**
 * assumes pin config from config.h, joystick facing cables forward, screen rotated landscape with long side on right.
 */
Arduino::Arduino(PointMode pointMode)
    : screen(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET),
      touch(XP, YP, XM, YM, RX)
{
    this->pointMode = pointMode;
    Serial.begin(115200); // general setup
    pinMode(JS_B, INPUT);
    screen.reset();
    screen.begin(0x7575); // id for xc4630
    screen.setRotation(1);
}

Point Arduino::getPoint()
{
    switch (pointMode)
    {
    case NO_INPUT:
        return Point(); // invalid by default
    case TOUCH_SCREEN:
    {
        TSPoint p = touch.getPoint();
        // rotate point to match Adafruit coordinates
        int16_t real_x = p.x;
        p.x = p.y;
        p.y = real_x;
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        p.x = map(p.x, TS_MINX, TS_MAXX, 0, screen.width()); // rescale to tft screen
        p.y = map(p.y, TS_MINY, TS_MAXY, 0, screen.height());
        return Point(p.x, p.y, p.z, p.z <= 0); // invalid if no pressure
    }
    case JOYSTICK_ABSOLUTE:
    {
        int b = digitalRead(JS_B);
        int x = map(analogRead(JS_X), JS_LD, JS_RU, 0, screen.width());
        int y = screen.height() - map(analogRead(JS_Y), JS_LD, JS_RU, 0, screen.height()); // also remap -y for coordinate system where +y is down
        return Point(x, y, b, b <= 0);                                                     // invalid if button not pressed
    }
    case JOYSTICK_ACCUMULATING:
    {
        static float acc_x;
        static float acc_y;
        static long last_call = micros();
        float delta_time = (micros() - last_call) / 1E6;
        int b = digitalRead(JS_B);
        int r_x = map(analogRead(JS_X), JS_LD, JS_RU, -JS_ACCUM_FAC, JS_ACCUM_FAC);
        int r_y = -map(analogRead(JS_Y), JS_LD, JS_RU, -JS_ACCUM_FAC, JS_ACCUM_FAC); // must also negate
        // we also apply a deadband to prevent small movement
        r_x = deadband(r_x, JS_ACCUM_DEADBAND, JS_ACCUM_FAC);
        r_y = deadband(r_y, JS_ACCUM_DEADBAND, JS_ACCUM_FAC);
        // multiply by a delta time (sec)
        acc_x += r_x * delta_time;
        acc_y += r_y * delta_time;
        last_call = micros();
        // clamp accumulation to screen
        acc_x = constrain(acc_x, 0, screen.width());
        if (acc_x == 0 || acc_x == screen.width())
            r_x = 0;
        acc_y = constrain(acc_y, 0, screen.height());
        if (acc_y == 0 || acc_y == screen.height())
            r_y = 0;
        // will be rounded by the int narrowing, hence we need to store the accumulated floats here
        return Point(acc_x, acc_y, b, near(r_x, 0, JS_ACCUM_DEADBAND) && near(r_y, 0, JS_ACCUM_DEADBAND)); // invalid if no delta
    }
    }
}

template <typename T>
void Arduino::draw_function(T func, uint16_t color, int lower_x = LX, int upper_x = UX, int lower_y = LY, int upper_y = UY, float delta_step = DT)
{
    // map by double to preserve decimal accuracy of the function, the only narrowing should occur at the screen
    // the built in function map() only applies to longs which removes all decimals via narrowing
    double last_x = map_double(lower_x, lower_x, upper_x, 0, screen.width());
    double last_y = map_double(func(lower_x), lower_y, upper_y, screen.height(), 0);
    for (double t = lower_x + delta_step; t <= upper_x; t += delta_step)
    {
        double x = map_double(t, lower_x, upper_x, 0, screen.width());
        double y = map_double(func(t), lower_y, upper_y, screen.height(), 0);
        // don't bother rendering what we can't see or will overflow from narrowing
        if (x >= 0 && x < screen.width() && y >= 0 && y < screen.height())
        {
            screen.drawLine(last_x, last_y, x, y, color);
        }
        last_x = x;
        last_y = y;
    }
}

void Arduino::reset_screen()
{
    screen.fillScreen(0);
}