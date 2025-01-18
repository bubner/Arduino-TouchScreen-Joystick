#pragma once
#define LCD_CS A3    // Chip Select goes to Analog 3
#define LCD_CD A2    // Command/Data goes to Analog 2
#define LCD_WR A1    // LCD Write goes to Analog 1
#define LCD_RD A0    // LCD Read goes to Analog 0
#define LCD_RESET 10 // wired A4 to D10

#define JS_X A4 // Analog 4: X -> joystick X axis
#define JS_Y A5 // Analog 5: Y -> joystick Y axis
#define JS_B 11 // Digital 11: B -> joystick button

#define JS_RU 0 // right and up value for x or y
#define JS_LD 1023 // left and down for x or y
#define JS_DEADBAND 10 // deadband around new 0, remapped to screen
#define JS_ACCUM_FAC 100 // accumulation factor (pixels/sec)
#define JS_ACCUM_DEADBAND 2 // accumulation deadband

#define YP A3 // touch controls shared on lcd data lines and lcd command lines
#define XM A2
#define YM 9
#define XP 8
#define RX 300 // rx is screen resistance in ohms (between x+ and x-) for pressure sensing

#define TS_MINX 90 // touch screen raw bounds
#define TS_MINY 70
#define TS_MAXX 950
#define TS_MAXY 900

#define MIDDLE_X 160 // middle of the screen in adafruit coordinates
#define MIDDLE_Y 120
