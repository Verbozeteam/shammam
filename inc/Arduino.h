#pragma once

#ifndef __ARDUINO_H__
#define __ARDUINO_H__

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

#endif
