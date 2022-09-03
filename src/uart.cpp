#include "uart.hpp"
#include "helpers.hpp"
#include "globalValues.hpp"

#include <stdlib.h>

using namespace Uart;

int uartFilestream;

int Uart::openUart(char * file_path){
    uartFilestream = -1;
    uartFilestream = open(file_path, O_RDWR | O_NOCTTY | O_NDELAY);      //Open in non blocking read/write mode
    if (uartFilestream == -1)
    {
        printf("Erro - Não foi possível iniciar a UART.\n");
        return uartFilestream;
    }
    else
    {
        if(debug) printf("UART inicializada!\n");
    }    
    struct termios options;
    tcgetattr(uartFilestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;     //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uartFilestream, TCIFLUSH);
    tcsetattr(uartFilestream, TCSANOW, &options);

    return uartFilestream;
}

ssize_t Uart::sendData(unsigned char * txBuffer, ssize_t sizeBuffer){
    if (uartFilestream != -1){
        if(debug) printf("Escrevendo caracteres na UART ...");
        ssize_t count = write(uartFilestream, txBuffer, sizeBuffer);
        if (count < 0){
            printf("UART TX error\n");
            return count;
        }
        else{
            if(debug) printf("Escrito.\n");
            return 0;
        }
    }
    
    printf("Filestream da UART inválido\n");
    return -1;
}

ssize_t Uart::receiveData(unsigned char * rxBuffer, ssize_t _size){
    if (uartFilestream != -1){
        ssize_t actual_size = 0, expected_size = _size;
        int count = 100;
        unsigned char * aux_buffer = (unsigned char *) malloc(expected_size + 1);
        do{
            ssize_t rx_length = read(uartFilestream, (void *) aux_buffer, expected_size);
            if (rx_length < 0){
                // printf("Erro na leitura.\n"); //An error occured (will occur if there are no bytes)
                sleepMs(5);
                count--;
                continue;
                // return rx_length;
            }
            if (rx_length == 0){
                // if(debug) printf("Nenhum dado disponível.\n"); //No data waiting
                sleepMs(5);
                count--;
                continue;
                // return actual_size;
            }

            // Debug
            aux_buffer[rx_length] = '\0';
            if(debug){
                printf("%li Bytes lidos\n", rx_length);
                printArrHex(aux_buffer, rx_length);
            }

            memcpy(&rxBuffer[actual_size], aux_buffer, rx_length);
            actual_size += rx_length;
            expected_size -= rx_length;
            
        } while(expected_size && count);
        free(aux_buffer);
        return actual_size;
    }
    printf("Filestream da UART inválido\n");
    return -1;
}