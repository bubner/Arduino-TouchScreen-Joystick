#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>
#include "Duino.h"

/**
 * start arduino using the selected point recognition mode using config.h constants
 */
Arduino::Arduino(PointMode pointMode)
    : screen(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET),
      touch(TS_XP, TS_YP, TS_XM, TS_YM, TS_RX)
{
    this->pointMode = pointMode;
    Serial.begin(SERIAL_BAUD);
    pinMode(JS_B, INPUT);
    screen.reset();
    screen.begin(LCD_BOARD_ID);
    screen.setRotation(LCD_BOARD_ROTATION);
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
        pinMode(TS_XM, OUTPUT);
        pinMode(TS_YP, OUTPUT);
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

void Arduino::reset_screen()
{
    screen.fillScreen(0);
}

void Arduino::set_joystick_accumulation_origin(int x, int y)
{
    acc_x = x;
    acc_y = y;
}

Point::Point(int x, int y, int z, bool invalid)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->invalid = invalid;
    this->uninitialised = false;
}

Point::Point()
{
    this->invalid = true;
    this->uninitialised = true;
}

bool Point::operator==(const Point &other)
{
    return near(x, other.x, POINT_EQUALITY_TOLERANCE_PX) && near(y, other.y, POINT_EQUALITY_TOLERANCE_PX) && z == other.z && invalid == other.invalid;
}

void Point::operator+=(const Point &other)
{
    this->x += other.x;
    this->y += other.y;
    this->z = other.z; // z axis does not need to be added
    this->invalid = other.invalid;
    this->uninitialised = false;
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

int deadband(int value, int deadband, int maxMag)
{
    if (abs(value) > deadband)
    {
        if (maxMag / deadband > 1.0e12)
        {
            return value > 0 ? value - deadband : value + deadband;
        }
        if (value > 0)
        {
            return maxMag * (value - deadband) / (maxMag - deadband);
        }
        else
        {
            return maxMag * (value + deadband) / (maxMag - deadband);
        }
    }
    else
    {
        return 0;
    }
}

bool near(int a, int b, int tol)
{
    return abs(a - b) < tol;
}

uint16_t rgb(uint16_t r, uint16_t g, uint16_t b)
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

double map_double(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
