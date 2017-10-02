# shammam
Want to write testing code for your Arduino project? shamamm is the perfect (very (very) simple) emulator for that!

# About
shammam basically compiles your Arduino code using the `g++` compiler. shammam runs an RPC server to allow you to interact with the emulated Arduino and emulates the Arduino's Serial using a socket server.

You will probably have some third-party libraries used in your Arduino code. Shammam allows you to write dummy stubs for those libraries (in C++) and implement RPC services to allow your (external) testing framework code to interact with your Arduino code. For example: Say you have Arduino code that uses DallasTemperature (an Arduino library for temperature sensor reading). To test that code, you can write a dummy implementation of that library such that when your normal Arduino code tries to read temperature, your dummy implementation always returns a certain value. Then you can easily write an RPC service that sets what that dummy temperature reading is. Now you have effectively controlled a "hypothetical" temperature sensor from outside your Arduino code, and your Arduino code will act as if that was an actual temperature reading.

gRPC allows you to write your testing code in any language (see [supported languages](https://grpc.io/)).

# Installation
- shammam needs [gRPC](https://grpc.io/docs/quickstart/cpp.html) to run. Make sure you also install Protocol Buffers (as explained in the link).
- A working g++ installation.

# Usage
`shammam -s <your source files>`
Type `shammam --help` for all the options.
