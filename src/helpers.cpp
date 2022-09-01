#include "helpers.hpp"
#include "crc16.hpp"
#include "globalValues.hpp"

#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <algorithm>
#include <chrono>
#include <thread>

// Time
uint64_t getTimeMs(){
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

void sleepMs(int t){
    std::this_thread::sleep_for(std::chrono::milliseconds(t));
}

// WiringPi
void setResistorPotency(int value){
    softPwmWrite(RESISTOR, std::min(value, 100));
}

void setFanPotency(int value){
    if(value > 0)
        softPwmWrite(VENTOINHA, std::max(std::min(value, 100), 40));
    else
        softPwmWrite(VENTOINHA, 0);
}

bool somethingOn(){
    return digitalRead(RESISTOR) || digitalRead(VENTOINHA);
}

// Modbus
ssize_t getSize(unsigned char * begin, unsigned char * end){
    return end - begin;
}

void addCRC(unsigned short crc, void * dest){
    memcpy(dest, &crc, sizeof(unsigned short));
}

bool checkCRC(unsigned char * message, ssize_t size){
    unsigned short crc_atual;
    ssize_t _size = sizeof(unsigned short);
    memcpy(&crc_atual, &message[size - _size], _size);
    return crc_atual == calcula_CRC(message, size - _size);
}

bool checkErrorBit(unsigned char * message){
    unsigned char cmd = message[1];
    unsigned char mask = 0x80;
    return (cmd & mask) != 0x80;
}

bool checkReceivedData(
    unsigned char * message,
    unsigned char expectedAddr,
    unsigned char expectedCode,
    unsigned char expectedSubCode
){
    if(message[0] != expectedAddr)    return false;
    if(message[1] != expectedCode)    return false;
    if(message[2] != expectedSubCode) return false;
    return true;
}

void addMatricula(void * dest){
    memcpy(dest, MATRICULA, MATRICULA_SIZE);
}

void printArrHex(unsigned char * arr, ssize_t size){
    for(ssize_t i=0; i < size; ++i)
        printf("%#x ", arr[i]);
    printf("\n");
}