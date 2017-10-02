#pragma once

#include <OneWire.h>

class DallasTemperature {
public:
    DallasTemperature() {} // dummy
    DallasTemperature(OneWire* ow) {} // dummy

    void begin() {} // dummy

    void requestTemperatures() {} // dummy

    float getTempCByIndex(int i);
};