// To run this example: python shammam.py -s <this file> src/* -i inc/ -p custom_protocol.proto

#include <OneWire.h>
#include <DallasTemperature.h>

OneWire one_wire(53);
DallasTemperature sensors(&one_wire);

void setup() {
    Serial.begin(9600);
    sensors.begin();
}

void loop() {
    sensors.requestTemperatures(); // Send the command to get temperatures (empty stub in shammam)
    float reading = sensors.getTempCByIndex(0);
    Serial.println(reading);
}
