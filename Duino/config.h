/* configuration options and hardware mapping for Duino */

// common colours
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

// default assumes pin config here, tested as joystick facing cables forward,
// screen rotated landscape with long side on right (xc4630 + ks0008)

#define SERIAL_BAUD 115200
#define LCD_BOARD_ID 0x7575  // id for xc4630 - sometimes attainable with .readID() on a Adafruit_TFTLCD object
#define LCD_BOARD_ROTATION 1 // gfx rotation (0-3)

#define LCD_CS A3    // LCD Chip Select pin
#define LCD_CD A2    // LCD Command/Data pin
#define LCD_WR A1    // LCD Write pin
#define LCD_RD A0    // LCD Read pin
#define LCD_RESET 10 // LCD Reset pin, usually wired to A4 but is D10 here

#define JS_X A4 // X -> joystick X axis
#define JS_Y A5 // Y -> joystick Y axis
#define JS_B 11 // B -> joystick button
// tuned for KS0008
#define JS_RU 0             // right and up value for x or y
#define JS_LD 1023          // left and down for x or y
#define JS_DEADBAND 10      // deadband around new 0, remapped to screen
#define JS_ACCUM_FAC 100    // accumulation factor (pixels/sec)
#define JS_ACCUM_DEADBAND 2 // accumulation deadband

// touch controls here are shared on lcd data lines and lcd command lines
#define TS_YP A3  // y-plus pin
#define TS_XM A2  // x-minus pin
#define TS_YM 9   // y-minus pin
#define TS_XP 8   // x-plus pin
#define TS_RX 300 // rx is screen resistance in ohms (between x+ and x-) for pressure sensing
// tuned for XC4630, touch screen raw bounds from raw input
#define TS_MINX 90
#define TS_MINY 70
#define TS_MAXX 950
#define TS_MAXY 900

#define SCREEN_MIDDLE_X 160 // middle of the screen in adafruit coordinates (post mapping)
#define SCREEN_MIDDLE_Y 120

#define POINT_EQUALITY_TOLERANCE_PX 2 // point x and y tolerance for ==

// default bounds for Duino draw_function, standard doubles for dynamic modification.
static double DF_LX = -100;  // lower x
static double DF_UX = 100;   // upper x
static double DF_LY = -100;  // lower y
static double DF_UY = 100;   // upper y
static double DF_DT = 0.01f; // delta step
