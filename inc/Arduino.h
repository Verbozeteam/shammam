#ifndef __SHAMMAM_SIMULATION__
#define __SHAMMAM_SIMULATION__

#include <Serial.h>
#include <stdlib.h>

extern SerialClass Serial;

#ifdef __cplusplus
extern "C"{
#endif

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 1
#define FALLING 2
#define RISING 3

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

typedef bool boolean;
typedef unsigned char byte;

void setup(void);
void loop(void);

#define NOT_A_PIN 0
#define NOT_A_PORT 0

#define NOT_AN_INTERRUPT -1

#ifdef __cplusplus
} // extern "C"
#endif

using std::max;
using std::min;

#ifndef NULL
#define NULL 0
#endif

unsigned long millis();
unsigned long micros();
void delay(unsigned long);
void delayMicroseconds(unsigned int us);

void attachInterrupt(uint8_t, void (*)(void), int mode);
void detachInterrupt(uint8_t);

void pinMode(int pin, int mode);
void digitalWrite(int  pin, int output);
void analogWrite(int pin, int output);
int analogRead(int pin);
int digitalRead(int pin);

int __get_pin_state(int type, int pin);
void __set_pin_state(int type, int pin, int state);
void __reset_pins();

#endif
