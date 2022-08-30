#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wiringPi.h>

#include "consts.hpp"
#include "helpers.hpp"
#include "uart.hpp"
#include "crc16.hpp"
#include "modbus.hpp"
#include "pid.hpp"

void handleUserInput(int userCmd){
    unsigned char byte;
    if(turnOn){
        switch (userCmd){
            case 0x02:
                turnOn = false;
                byte = 0;
                sendData(CMD_ENVIA_ESTADO_SISTEMA, byte);
                break;
            case 0x03:
                heating = true;
                byte = 1;
                sendData(CMD_ENVIA_ESTADO_FUNCIONAMENTO, byte);
                break;
            case 0x04:
                heating = false;
                byte = 0;
                sendData(CMD_ENVIA_ESTADO_FUNCIONAMENTO, byte);
                break;
            case 0x05:
                timer++;
                break;
            case 0x06:
                timer--;
                break;
            case 0x07:
                printf("TODO\n");
                break;
            default:
                printf("Comando não listado.\n");
        }
    }
    else if(userCmd == 0x01){
        turnOn = true;
        byte = 1;
        sendData(CMD_ENVIA_ESTADO_SISTEMA, byte);
    }
}

int main(int argc, const char * argv[]) {
    if(setPins() == -1)
        return -1;

    pid_configura_constantes(30.0, 0.2, 400.0);

    char uartPath[] = "/dev/serial0";
    int uartFilestream = Uart::openUart(uartPath);
    while(run){
        sendDataRequest(CMD_SOLICITA_COMANDO_USUARIO);
        sleepMs(TEMPO_ENTRE_REQUEST);
        int userCmd;
        ssize_t rx_length = receiveData(CMD_SOLICITA_COMANDO_USUARIO, &userCmd);
        if(rx_length > 0 && userCmd != 0){
            printf("Comando usuário: %#x\n", userCmd);
            handleUserInput(userCmd);
        }
        sleepMs(TEMPO_ENTRE_REQUEST);
    }

    close(uartFilestream);
    return 0;
}


    // sendDataRequest(CMD_SOLICITA_TEMPERATURA_INTERNA);
    // sleep(1);
    // float inTemp;
    // rx_length = receiveData(CMD_SOLICITA_TEMPERATURA_INTERNA, &inTemp);
    // printf("%li Bytes lidos; Temperatura interna: %f\n", rx_length, inTemp);
    // sleep(1);

    // sendDataRequest(CMD_SOLICITA_TEMPERATURA_REFERENCIA);
    // sleep(1);
    // float refTemp;
    // rx_length = receiveData(CMD_SOLICITA_TEMPERATURA_REFERENCIA, &refTemp);
    // printf("%li Bytes lidos; Temperatura de Referencia: %f\n", rx_length, refTemp);
    // sleep(1);

    // sendDataRequest(CMD_SOLICITA_COMANDO_USUARIO);
    // sleep(1);
    // int userCmd;
    // rx_length = receiveData(CMD_SOLICITA_COMANDO_USUARIO, &userCmd);
    // printf("%li Bytes lidos; Comando usuário: %#x\n", rx_length, userCmd);
    // sleep(1);

    // sendData(CMD_ENVIA_SINAL_CONTROLE, 50);
    // sleep(1);
    
    // sendData(CMD_ENVIA_SINAL_REFERENCIA, 35.5f);
    // sleep(1);
    
    // sendData(CMD_ENVIA_ESTADO_SISTEMA, 1);
    // sleep(1);
    // int systemState;
    // rx_length = receiveData(CMD_ENVIA_ESTADO_SISTEMA, &systemState);
    // printf("%li Bytes lidos; Estado do sistema: %d\n", rx_length, systemState);
    // sleep(1);

    // sendData(CMD_ENVIA_ESTADO_FUNCIONAMENTO, 1);
    // sleep(1);
    // int funcState;
    // rx_length = receiveData(CMD_ENVIA_ESTADO_FUNCIONAMENTO, &funcState);
    // printf("%li Bytes lidos; Estado de funcionamento: %d\n", rx_length, funcState);
    // sleep(1);

    // sendData(CMD_ENVIA_TEMPORIZADOR, 15);
    // sleep(1);
    // int timer;
    // rx_length = receiveData(CMD_ENVIA_TEMPORIZADOR, &timer);
    // printf("%li Bytes lidos; Estado de funcionamento: %d\n", rx_length, timer);
    // sleep(1);