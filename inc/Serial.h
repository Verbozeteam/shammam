#pragma once

#include <string>

using std::string;

class SerialClass {
    int m_socketfd;
    int m_port;

    int m_client_socketfd;

    string m_buffer;

public:
    SerialClass();
    void begin(int baud);
    void write(char c);
    void write(char* str, int len);
    int available();
    char read();

    void __update__();
};
