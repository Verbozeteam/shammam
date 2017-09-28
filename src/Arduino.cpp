#include <Arduino.h>
#include <Serial.h>
#include <iostream>
#include <unordered_map>

using namespace std;
using std::unordered_map;

SerialClass Serial;

unordered_map<int, int> g_analog_pin_map;
unordered_map<int, int> g_digital_pin_map;

void setup();
void loop();

long millis() {
    return 0;
}

void pinMode(int pin, int mode) {
}

void digitalWrite(int pin, int output) {
    g_digital_pin_map.insert(std::pair<int, int> (pin, output));
}

void analogWrite(int pin, int output) {
    g_digital_pin_map.insert(std::pair<int, int> (pin, output));
}

int digitalRead(int pin) {
    if (g_digital_pin_map.find(pin) == g_digital_pin_map.end())
        analogWrite(pin, 0);
    return g_digital_pin_map.find(pin)->second;
}

int analogRead(int pin) {
    if (g_analog_pin_map.find(pin) == g_analog_pin_map.end())
        analogWrite(pin, 0);
    return g_analog_pin_map.find(pin)->second;
}

int main(int argc, char* argv[]) {
    std::cout << "Shammam v1.0.0" << std::endl;

    setup();
    while (1) {
        Serial.__update__();
        loop();
    }
}
