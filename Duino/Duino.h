#pragma once
#ifndef DUINO_H
#define DUINO_H

#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>
#include "config.h"

/**
 * common 2d geometry
 */
class Point
{
public:
    // a point follows the Adafruit GFX graphics library coordinate system, where +x is right, +y is down, (0,0) is in the top left.
    int x, y, z;
    bool invalid;
    /**
     * whether this point is the result of a default constructor and the values held should not be respected
     */
    bool uninitialised;
    Point(int x, int y, int z = 0, bool invalid = true);
    Point();
    bool operator==(const Point &other);
    void operator+=(const Point &other);
    /**
     * print to Serial
     */
    void print();
};

/**
 * representations of Point
 */
enum PointMode
{
    NO_INPUT,
    TOUCH_SCREEN,
    JOYSTICK_ABSOLUTE,
    JOYSTICK_ACCUMULATING
};

/**
 * main class for interacting with the arduino
 */
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
    template <typename T> // must add definition here due to use of generics
    void draw_function(T func, uint16_t color, int lower_x = DF_LX, int upper_x = DF_UX, int lower_y = DF_LY, int upper_y = DF_UY, float delta_step = DF_DT)
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
    Arduino(PointMode pointMode);
    /**
     * shorthand for `arduino->screen.fillScreen(rgb(0, 0, 0));`
     */
    void reset_screen();
    /**
     * sets a new accumulation origin. no-ops in any other mode than `JOYSTICK_ACCUMULATING`
     */
    void set_joystick_accumulation_origin(int x, int y);
private:
    int acc_x = 0, acc_y = 0; // only used for JOYSTICK_ACCUMULATING
};

/**
 * remap value around 0 by deadband
 */
int deadband(int value, int deadband, int maxMag);
/**
 * test if a and b are within tol
 */
bool near(int a, int b, int tol);
/**
 * convert rgb values to a packed integer
 */
uint16_t rgb(uint16_t r, uint16_t g, uint16_t b);
/**
 * map function retaining double accuracy
 */
double map_double(double x, double in_min, double in_max, double out_min, double out_max);

#endif
