#ifndef __ARDUINO_SIMULATION__
#define __ARDUINO_SIMULATION__

#include <Serial.h>

extern SerialClass Serial;

#ifndef NULL
#define NULL 0
#endif

long millis();
void pinMode(int pin, int mode);
void digitalWrite(int  pin, int output);
void analogWrite(int pin, int output);
int analogRead(int pin);
int digitalRead(int pin);

int __get_pin_state(int type, int pin);
void __set_pin_state(int type, int pin, int state);
void __reset_pins();

void runRPC(string addr);

#endif
