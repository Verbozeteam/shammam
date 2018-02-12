#pragma once

#include <string>

using std::string;

class SerialClass {
    int m_socketfd;
    int m_port;
    bool m_is_client;

    int m_client_socketfd;

    string m_buffer;

public:
    SerialClass(int port = 9911, bool isClient = false);
    void begin(int baud);

    void print(char val);
    void print(int val);
    void print(float val);
    void print(char* str);

    void println(char val);
    void println(int val);
    void println(float val);
    void println(char* str);

    void write(char c);
    void write(char* str, int len);

    int available();
    char read();

    void __update__();
};
