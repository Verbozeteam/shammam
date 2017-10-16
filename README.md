# shammam
Want to write testing code for your Arduino project? shamamm is the perfect (very (very) simple) emulator for that!

# About
shammam basically compiles your Arduino code using the `g++` compiler. shammam runs an RPC server to allow you to interact with the emulated Arduino and emulates the Arduino's Serial using a socket server.

You will probably have some third-party libraries used in your Arduino code. Shammam allows you to write dummy stubs for those libraries (in C++) and implement RPC services to allow your (external) testing framework code to interact with your Arduino code. For example: Say you have Arduino code that uses DallasTemperature (an Arduino library for temperature sensor reading). To test that code, you can write a dummy implementation of that library such that when your normal Arduino code tries to read temperature, your dummy implementation always returns a certain value. Then you can easily write an RPC service that sets what that dummy temperature reading is. Now you have effectively controlled a "hypothetical" temperature sensor from outside your Arduino code, and your Arduino code will act as if that was an actual temperature reading.

gRPC allows you to write your testing code in any language (see [supported languages](https://grpc.io/)).

# Installation
- shammam needs [gRPC](https://grpc.io/docs/quickstart/cpp.html) to run. Make sure you also install Protocol Buffers (as explained in the link).
- A working g++ installation.
- pkg-config (`brew install pkg-config` on Mac or `sudo apt-get install pkg-config` on Linux)

# Usage
`shammam -s <your source files>`
Type `shammam --help` for all the options.

# Neat stuff
shammam will create a binary in your directory which runs the emulator. Of course, this allows you to run gdb and valgrind, on your Arduino code! NEAT!

shammam is compiled using g++, which means you may use any standard libraries or even third party libraries while debugging! NEAT! (make sure you don't try to compile these when putting the code on an actual Arduino... use the `__SHAMMAM_SIMULATION__` to detect if the code is being compiled using shammam).

# The serial port
shammam implements a basic `Serial` replacement that emulates the serial communication but on a socket hosted (by default) on 0.0.0.0:9911. So any Arduino code that will do `Serial.write` will write to that socket (if someone is connected to it) and anything written to the socket by a client will be available for `Serial.read()` (and `Serial.available()`).

# Advanced usage
You will probably need more than the default setup to have a useful test suite. For that, it will be helpful to understand how shammam works.

## shammam internals
The idea is very simple: Imagine you just ran g++ on your .ino file. It would work, except that it will complain about missing stuff, like `pinMode`, `millis`, `Serial`, etc... So shammam basically defines part of these for you and allows you to define the rest. By doing so, shammam allows you to completely control the "underlying layer" of the Arduino code (pin states, library functions behaviors, etc...). shammam does the following:
- shammam takes your source code and augments it with internal code that defines basic Arduino stuff (pin stuff, `millis`, etc...).
- shammam has a special definition for `Serial` where it hosts a local socket server for your convenience so you could emulate the serial communication if you want to. If you just want to see the serial output and write to it manually, you can run netcat: `nc localhost 9911`
- shammam runs a gRPC server on 0.0.0.0:5001. The RPC server implements 3 functions: SetPinState, GetPinState and ResetPins. Those can be used to (artificially, from testing code...) to set a "hypothetical" sensor reading or check pin values to make sure your Arduino code is behaving properly
- the RPC server is run ASYNCHRONOUSLY! make sure you make any code inside them is thread-safe and they interfere with your other shammam code safely.

## Implementing other Ardunio dependencies
Simply implement any header files that your Ardunio code expects as libraries (e.g. SPI.h, DallasTemperature.h, etc...) by mirroring the declarations expected. Then you can implement those functions stubs however you want: you may leave them empty or have them return a value you want.

A very useful technique is to make those stubs return values globally declared and controlled by RPC functions. For example, consider the following code:

```c++
#include <RPC.h>
#include "DallasTemperature.h" // A header file that mimics the actual DallasTemperature.h library header

float g_temperature = 0.0f; // global "current sensor reading"

/**
 * So any time the Arduino code calls getTempCByIndex() it will get the value g_temperature...
 */
float DallasTemperature::getTempCByIndex(int i) {
    return g_temperature; // doesn't need a lock
}

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

ArduinoServiceBasicImpl* __get_rpc_service() {
    return new CustomArduinoService();
}
```

The above code basically replaces the `DallasTemperature::getTempCByIndex()` function by one that always returns a set value that can be controlled using an RPC call. See the `extend_rpc` example for details.

You need to define the appropriate protocol using the Protocol Buffers format (.proto file). Check out the sample file `arduino_protocol.proto` and the extended one `examples/extend_rpc/custom_protocol.proto` or refer to the docs of [Protocol Buffers v3](https://developers.google.com/protocol-buffers/docs/proto3).

## Using RPCs
To communicate with the RPC hosted by shammam, you need to:
- For a given language use protoc (the Protocol Buffer compiler) to generate the appropriate protocol compiled code for that language. Use the same protocol (.proto) file for this as the one you used to run the shammam RPC (if you didn't specify any the default is `arduino_protocol.proto`)
- Use the grpc client implementation for that language. Find more [here](https://github.com/grpc/grpc/tree/v1.6.x/examples).
