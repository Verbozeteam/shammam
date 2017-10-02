#include <Serial.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>

SerialClass::SerialClass() {
    m_port = 9911;
    m_socketfd = -1;
    m_client_socketfd = -1;
}

void SerialClass::begin(int baud) {
    struct sockaddr_in address;
    int opt = 1;

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
}

void SerialClass::write(char c) {
    if (m_client_socketfd != -1)
        send(m_client_socketfd, &c, 1, 0);
    // printf("Writing %u\n", ((int)c) & 0xFF);
}

void SerialClass::write(char* str, int len) {
    // printf("Writing...");
    // for (int i = 0; i < len; i++)
    //     printf(" %u", ((int)str[i]) & 0xFF);
    // printf("\n");
    if (m_client_socketfd != -1)
        send(m_client_socketfd, str, len, 0);
}

int SerialClass::available() {
    // if (m_buffer.length() > 0)
    //     printf("%d bytes available!\n", (int)m_buffer.length());
    return m_buffer.length();
}

char SerialClass::read() {
    while (!m_buffer.length())
        __update__();

    char c = m_buffer[0];
    m_buffer = m_buffer.substr(1);
    // printf("Reading %u\n", ((int)c) & 0xFF);
    return c;
}

void SerialClass::__update__() {
    if (m_socketfd != -1) {
        timeval tv = {0};

        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(m_socketfd, &read_fds);
        if (m_client_socketfd != -1)
            FD_SET(m_client_socketfd, &read_fds);

        int maxfd = m_socketfd > m_client_socketfd ? m_socketfd : m_client_socketfd;

        int ret = select(maxfd + 1, &read_fds, NULL, NULL, &tv);
        if (ret > 0) {
            if (FD_ISSET(m_socketfd, &read_fds)) {
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
