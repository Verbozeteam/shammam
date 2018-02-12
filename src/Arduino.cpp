#include <Arduino.h>
#include <Serial.h>
#include <RPC.h>
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
float g_time_multiplier = 1.0;

void setup();
void loop();

unsigned long millis() {
    static long long start_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    long long cur_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    cur_time -= start_time;
    return (long) ((float)cur_time * g_time_multiplier);
}

void delay(unsigned long) {

}

void delayMicroseconds(unsigned int us) {

}

void attachInterrupt(uint8_t, void (*)(void), int mode) {

}

void detachInterrupt(uint8_t) {

}

uint8_t digitalPinToInterrupt(uint8_t pin) {
    return pin;
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
    unordered_map<string, std::pair<string, string>> params = {
        // socket hosting parameters
        std::pair<string, std::pair<string, string>>("rpc_port", std::pair<string, string>("5001",
            "Port to host the rpc service (default is 5001)")),
        std::pair<string, std::pair<string, string>>("serial_port", std::pair<string, string>("9911",
            "Port to host the serial communication (default is 9911)")),

        // emulation parameters
        std::pair<string, std::pair<string, string>>("time", std::pair<string, string>("1",
            "Time multiplier (default is 1)")),

        // misc.
        std::pair<string, std::pair<string, string>>("help", std::pair<string, string>("", "This menu")),
    };

    unordered_map<string, string> params_clean;
    for (auto it = params.begin(); it != params.end(); it++)
        params_clean.insert(std::pair<string, string>(it->first, it->second.first));

    bool stop = false;
    for (int i = 1; i < argc; i++) {
        char* arg = argv[i];
        string param = "";
        if (i != argc - 1)
            param = argv[i+1];
        if (strlen(arg) < 3 || arg[0] != '-' || arg[1] != '-') {
            std::cout << "Unknown argument " << arg << std::endl;
            stop = true;
        } else {
            auto params_iter = params_clean.find(arg + 2 /* Skip the '--' */);
            if (params_iter == params_clean.end()) {
                std::cout << "Unknown argument " << arg << std::endl;
                stop = true;
            } else {
                params_iter->second = param;
                if (strcmp(arg, "--help") != 0)
                    i++;
                else
                    stop = true;
            }
        }
    }

    if (argc == 2 && strcmp(argv[1], "--help") == 0)
        stop = true;

    if (stop) {
        string err = "Usage: " + string(argv[0]) + " [options]\n";
        err += "Options:\n";
        for (auto it = params.begin(); it != params.end(); it++)
            err += "\t--" + it->first + " <arg>: " + it->second.second + " (default is " + it->second.first + ")\n";
        std::cout << err;
        return 1;
    }

    g_time_multiplier = atof(params_clean.find("time")->second.c_str());
    int rpc_port = atoi(params_clean.find("rpc_port")->second.c_str());
    Serial = SerialClass(atoi(params_clean.find("serial_port")->second.c_str()));

    std::cout << "Shammam v1.0.0" << std::endl;
    std::cout << "Running with time multiplier: " << g_time_multiplier << std::endl;

    runRPC("0.0.0.0:"+std::to_string(rpc_port));

    setup();
    while (1) {
        Serial.__update__();
        loop();
    }

    return 0;
}
