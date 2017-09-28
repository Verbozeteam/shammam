#pragma once

#include <OneWire.h>

class DallasTemperature {
public:
    DallasTemperature() {}
    DallasTemperature(OneWire* ow) {
    }

    void begin() {}

    void requestTemperatures() {}

    float getTempCByIndex(int i) {
        return 0.0f;
    }
};