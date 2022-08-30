#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <string.h>
#include <stdlib.h>
#include <cstdint>

// Time
uint64_t getTimeMs();
void sleepMs(int t);

// WiringPi
int setPins();
void setResistorPotency(int value);
void setFanPotency(int value);

// Modbus
ssize_t getSize(unsigned char * begin, unsigned char * end);
void addCRC(unsigned short crc, void * dest);
bool checkCRC(unsigned char * message, ssize_t size);
bool checkErrorBit(unsigned char * message);
bool checkReceivedData(
    unsigned char * message,
    unsigned char expectedAddr,
    unsigned char expectedCode,
    unsigned char expectedSubCode
);
void addMatricula(void * dest);
void printArrHex(unsigned char * arr, ssize_t size);

#endif