#ifndef UART_HPP
#define UART_HPP

#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include "helpers.hpp"

namespace Uart {
    int openUart(char * file_path);
    ssize_t sendData(unsigned char * txBuffer, ssize_t sizeBuffer);
    ssize_t receiveData(unsigned char * rxBuffer, ssize_t _size);
}

#endif