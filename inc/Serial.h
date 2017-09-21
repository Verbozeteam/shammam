#pragma once

class SerialClass {
public:
    SerialClass() {
    }

    void begin(int baud) {

    }

    void write(char c) {

    }

    void write(char* str, int len) {

    }

    int available() {
        return 0;
    }

    char read() {
        return ' ';
    }
};

extern SerialClass Serial;
