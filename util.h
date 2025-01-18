#pragma once
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

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