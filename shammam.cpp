
#include <iostream>
#include <Serial.h>
#include <Misc.h>

using namespace std;

SerialClass Serial;

void setup();
void loop();

int main(int argc, char* argv[]) {
    std::cout << "Shammam v1.0.0" << std::endl;
    setup();
    while (1) {
        loop();
    }
}
