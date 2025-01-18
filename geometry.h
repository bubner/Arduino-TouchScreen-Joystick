#pragma once
#include "util.h"

#define POINT_TOLERANCE_PX 2 // equality equivalence

class Point
{
public:
    // a point follows the Adafruit GFX graphics library coordinate system, where +x is right, +y is down, (0,0) is in the top left.
    int x, y, z;
    bool invalid;
    Point(int x = 0, int y = 0, int z = 0, bool invalid = true);
    bool operator==(const Point &other);
    void operator+=(const Point &other);
    void print();
};

Point::Point(int x, int y, int z, bool invalid)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->invalid = invalid;
}

bool Point::operator==(const Point &other)
{
    return near(x, other.x, POINT_TOLERANCE_PX) && near(y, other.y, POINT_TOLERANCE_PX) && z == other.z && invalid == other.invalid;
}

void Point::operator+=(const Point &other)
{
    this->x += other.x;
    this->y += other.y;
    this->z = other.z; // z axis does not need to be added
}

void Point::print()
{
    char buffer[16];
    sprintf(buffer, "X = %03d", this->x);
    Serial.print(buffer);
    sprintf(buffer, "\tY = %03d", this->y);
    Serial.print(buffer);
    sprintf(buffer, "\tZ = %03d", this->z);
    Serial.print(buffer);
    Serial.println(this->invalid ? " (invalid)" : "");
}