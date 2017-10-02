#include <Arduino.h>
#include <Serial.h>
#include <iostream>
#include <unordered_map>
#include <chrono>
#include <mutex>

using namespace std;
using std::unordered_map;
using namespace std::chrono;

SerialClass Serial;

std::mutex g_pins_mutex;
unordered_map<int, int> g_analog_pin_map;
unordered_map<int, int> g_digital_pin_map;

void setup();
void loop();

long millis() {
    static long long start_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    long long cur_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    cur_time -= start_time;
    return (long) cur_time;
}

void pinMode(int pin, int mode) {
}

void digitalWrite(int pin, int output) {
    std::lock_guard<std::mutex> guard(g_pins_mutex);
    if (g_digital_pin_map.find(pin) != g_digital_pin_map.end())
        g_digital_pin_map.erase(pin);
    g_digital_pin_map.insert(std::pair<int, int> (pin, output));
}

void analogWrite(int pin, int output) {
    digitalWrite(pin, output);
}

int digitalRead(int pin) {
    std::lock_guard<std::mutex> guard(g_pins_mutex);
    if (g_digital_pin_map.find(pin) == g_digital_pin_map.end())
        g_digital_pin_map.insert(std::pair<int, int> (pin, 0));
    return g_digital_pin_map.find(pin)->second;
}

int analogRead(int pin) {
    std::lock_guard<std::mutex> guard(g_pins_mutex);
    if (g_analog_pin_map.find(pin) == g_analog_pin_map.end())
        g_analog_pin_map.insert(std::pair<int, int> (pin, 0));
    return g_analog_pin_map.find(pin)->second;
}

int __get_pin_state(int type, int pin) {
    if (type == 0) {
        return digitalRead(pin);
    } else {
        return analogRead(pin);
    }
}

void __set_pin_state(int type, int pin, int state) {
    unordered_map<int, int>* pin_map;
    if (type == 0)
        pin_map = &g_digital_pin_map;
    else
        pin_map = &g_analog_pin_map;

    std::lock_guard<std::mutex> guard(g_pins_mutex);
    if (pin_map->find(pin) != pin_map->end())
        pin_map->erase(pin);
    pin_map->insert(std::pair<int, int> (pin, state));
}

void __reset_pins() {
    std::lock_guard<std::mutex> guard(g_pins_mutex);
    g_digital_pin_map.clear();
    g_analog_pin_map.clear();
}

int main(int argc, char* argv[]) {
    std::cout << "Shammam v1.0.0" << std::endl;

    runRPC("0.0.0.0:5001");

    setup();
    while (1) {
        Serial.__update__();
        loop();
    }
}
