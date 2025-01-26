#pragma once
#ifndef DUINO_H
#define DUINO_H

#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>
#include "config.h"

/**
 * f(x)
 */
typedef double (*unary_function)(const double x);

/**
 * common 2d geometry
 */
class Point
{
public:
    // a point follows the Adafruit GFX graphics library coordinate system, where +x is right, +y is down, (0,0) is in the top left. z is used for touch/button pressure.
    int x, y, z;
    // whether this point has elected to be invalid (such as a point returned that isn't registered as an active movement)
    bool invalid;
    Point(int x, int y, int z, bool invalid) : x(x), y(y), z(z), invalid(invalid) {}
    Point(int x, int y) : x(x), y(y), z(0), invalid(true) {}
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
    /**
     * LCD screen, used to write and read from the display
     */
    Adafruit_TFTLCD screen;
    /**
     * raw touchscreen object; rarely used (use `TOUCH_SCREEN` point mode and `getPoint()`)
     */
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
     * draw an unary function e.g. `double f(double x) { return x; }` then `arduino->draw_function(f, WHITE);` will draw a white y=x function with default bounds
     */
    void draw_function(unary_function func, uint16_t color, int lower_x = DF_LX, int upper_x = DF_UX, int lower_y = DF_LY, int upper_y = DF_UY, float delta_step = DF_DT);
    /**
     * start arduino using the selected point recognition mode using config.h constants
     */
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

// TODO: next steps are to make a non-blocking fsm of some sort

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
