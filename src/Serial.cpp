#include <cstdio>
#include <cstring>
#include <Serial.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>
#include <arpa/inet.h>

SerialClass::SerialClass(int port, bool isClient) {
    m_port = port;
    m_is_client = isClient;
    m_socketfd = -1;
    m_client_socketfd = -1;
}

void SerialClass::begin(int baud) {
    if (!m_is_client) {
        struct sockaddr_in address;
        int opt = 1;

        printf("Serial communication on port %d\n", m_port);

        m_socketfd = socket(AF_INET, SOCK_STREAM, 0);
        if (!m_socketfd) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        // Forcefully attaching socket to the port 8080
        if (setsockopt(m_socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
        {
            perror("setsockopt failed");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(m_port);

        // Forcefully attaching socket to the port 8080
        if (bind(m_socketfd, (struct sockaddr *)&address,
                                     sizeof(address))<0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        if (listen(m_socketfd, 10) < 0)
        {
            perror("listen failed");
            exit(EXIT_FAILURE);
        }
    } else {
        const char* server_name = "localhost";
        const int server_port = m_port;

        struct sockaddr_in server_address;
        memset(&server_address, 0, sizeof(server_address));
        server_address.sin_family = AF_INET;

        // creates binary representation of server name
        // and stores it as sin_addr
        // http://beej.us/guide/bgnet/output/html/multipage/inet_ntopman.html
        inet_pton(AF_INET, server_name, &server_address.sin_addr);

        // htons: port in network order format
        server_address.sin_port = htons(server_port);

        // open a stream socket
        if ((m_client_socketfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
            printf("could not create socket\n");
            exit(EXIT_FAILURE);
        }

        // TCP is connection oriented, a reliable connection
        // **must** be established before any data is exchanged
        if (connect(m_client_socketfd, (struct sockaddr*)&server_address,
                    sizeof(server_address)) < 0) {
            printf("could not connect to server\n");
            exit(EXIT_FAILURE);
        }
    }
}

void SerialClass::print(char val) {
    this->write(val);
}

void SerialClass::print(int val) {
    char tmp[64];
    sprintf(tmp, "%d", val);
    this->write((char*)tmp, strlen(tmp));
}

void SerialClass::print(float val) {
    char tmp[64];
    sprintf(tmp, "%f", val);
    this->write((char*)tmp, strlen(tmp));
}

void SerialClass::print(char* str) {
    this->write(str, strlen(str));
}

void SerialClass::println(char val) {
    this->print(val);
    this->print('\n');
}

void SerialClass::println(int val) {
    this->print(val);
    this->print('\n');
}

void SerialClass::println(float val) {
    this->print(val);
    this->print('\n');
}

void SerialClass::println(char* str) {
    this->print(str);
    this->print('\n');
}

void SerialClass::write(char c) {
    if (m_client_socketfd != -1)
        send(m_client_socketfd, &c, 1, 0);
}

void SerialClass::write(char* str, int len) {
    if (m_client_socketfd != -1)
        send(m_client_socketfd, str, len, 0);
}

int SerialClass::available() {
    return m_buffer.length();
}

char SerialClass::read() {
    while (!m_buffer.length())
        __update__();

    char c = m_buffer[0];
    m_buffer = m_buffer.substr(1);
    return c;
}

void SerialClass::__update__() {
    if (m_socketfd != -1 || m_client_socketfd != -1) {
        timeval tv = {0};

        fd_set read_fds;
        FD_ZERO(&read_fds);
        if (m_socketfd != -1)
            FD_SET(m_socketfd, &read_fds);
        if (m_client_socketfd != -1)
            FD_SET(m_client_socketfd, &read_fds);

        int maxfd = m_socketfd > m_client_socketfd ? m_socketfd : m_client_socketfd;

        int ret = select(maxfd + 1, &read_fds, NULL, NULL, &tv);
        if (ret > 0) {
            if (m_socketfd != -1 && FD_ISSET(m_socketfd, &read_fds)) {
                if (m_client_socketfd != -1)
                    close(m_client_socketfd);
                struct sockaddr_in address;
                int addrlen;
                m_client_socketfd = accept(m_socketfd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
                // printf("Client connected!\n");
            }
            if (m_client_socketfd != -1 && FD_ISSET(m_client_socketfd, &read_fds)) {
                char buffer[1024];
                int len = recv(m_client_socketfd, buffer, 1024, 0);
                if (len == 0) {
                    close(m_client_socketfd);
                    m_client_socketfd = -1;
                    // printf("Client disconnected!\n");
                } else {
                    for (int i = 0; i < len; i++)
                        m_buffer += buffer[i];
                    // printf("Received smth!\n");
                }
            }
        }
    }
}
