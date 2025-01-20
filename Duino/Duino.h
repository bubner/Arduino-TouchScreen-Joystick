#pragma once
#ifndef DUINO_H
#define DUINO_H

#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>

/**
 * common 2d geometry
 */
class Point
{
public:
    // a point follows the Adafruit GFX graphics library coordinate system, where +x is right, +y is down, (0,0) is in the top left.
    int x, y, z;
    bool invalid;
    Point(int x = 0, int y = 0, int z = 0, bool invalid = true);
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
    template <typename T>
    void draw_function(T func, uint16_t color, int lower_x = LX, int upper_x = UX, int lower_y = LY, int upper_y = UY, float dt = DT);
    Arduino(PointMode pointMode);
    /**
     * shorthand for `arduino->screen.fillScreen(rgb(0, 0, 0));`
     */
    void reset_screen();
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
