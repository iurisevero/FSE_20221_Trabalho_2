#ifndef MODBUS_HPP
#define MODBUS_HPP

#include <stdlib.h>

ssize_t assembleProtocol(
    unsigned char * message,
    unsigned char cmd,
    bool isRequest=false
);
ssize_t sendDataRequest(unsigned char cmd);
ssize_t sendData(unsigned char cmd, int value);
ssize_t sendData(unsigned char cmd, float value);
ssize_t sendData(unsigned char cmd, unsigned char value);
ssize_t sendData(unsigned char cmd, char * value);
ssize_t sendAssembleData(unsigned char * message, ssize_t _size);
ssize_t receiveData(unsigned char cmd, int * value);
ssize_t receiveData(unsigned char cmd, float * value);
ssize_t receiveData(unsigned char cmd, char * value);
ssize_t receiveConstantSizeData(
    unsigned char * message,
    ssize_t expectedSize,
    unsigned char subCmd
);

#endif