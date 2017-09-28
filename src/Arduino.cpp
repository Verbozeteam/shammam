#include <Arduino.h>
#include <Serial.h>
#include <iostream>

using namespace std;

SerialClass Serial;

void setup();
void loop();

long millis() {
    return 0;
}

void pinMode(int pin, int mode) {

}

void digitalWrite(int  pin, int output) {

}

void analogWrite(int pin, int output) {

}

int analogRead(int pin) {
    return 0;
}

int digitalRead(int pin) {
    return 0;
}

int main(int argc, char* argv[]) {
    std::cout << "Shammam v1.0.0" << std::endl;
    setup();
    while (1) {
        loop();
    }
}
