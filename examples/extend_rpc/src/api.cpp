#include <RPC.h>
#include "DallasTemperature.h" // A header file that mimics the actual DallasTemperature.h library header

float g_temperature = 0.0f; // global "current sensor reading"

/**
 * So any time the Arduino code calls getTempCByIndex() it will get the value g_temperature...
 */
float DallasTemperature::getTempCByIndex(int i) {
    return g_temperature; // doesn't need a lock
}

/**
 * This extends ArduinoServiceBasicImpl (which implements SetPinState, GetPinState and ResetPins)
 * by defining a new RPC for setting the temperature
 */
class CustomArduinoService : public ArduinoServiceBasicImpl {
    Status SetTemperatureSensor(
        ServerContext* context,
        const shammam::Temperature* request,
        shammam::Empty* reply) {

        // g_temperature is set in the RPC call!
        g_temperature = request->temp(); // doesn't need a lock
        return Status::OK;
    }
};

/**
 * This function must be defined when using a custom protocol to tell shammam about the new RPC imeplementation
 */
ArduinoServiceBasicImpl* __get_rpc_service() {
    return new CustomArduinoService();
}
