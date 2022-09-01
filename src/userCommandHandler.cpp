#include <stdio.h>

#include "userCommandHandler.hpp"
#include "globalValues.hpp"
#include "helpers.hpp"
#include "modbus.hpp"
#include "timerHandler.hpp"
#include "i2cLCD.hpp"

void runUserCommand(){
    int userCmd;
    while(run){
        sendDataRequest(CMD_SOLICITA_COMANDO_USUARIO);
        sleepMs(TEMPO_ENTRE_REQUEST);
        ssize_t rx_length = receiveData(CMD_SOLICITA_COMANDO_USUARIO, &userCmd);
        if(rx_length > 0 && userCmd != 0){
            printf("Comando usuário: %#x\n", userCmd);
            handleUserInput(userCmd);
        }
        sleepMs(TEMPO_ENTRE_REQUEST);
    }
    printf("UserCommandHandler finished\n");
    return;
}

void handleUserInput(int userCmd){
    unsigned char byte;
    int ret;
    if(turnOn){
        switch (userCmd){
            case 0x02:
                turnOn = false;
                byte = 0;
                sendData(CMD_ENVIA_ESTADO_SISTEMA, byte);
                sleepMs(TEMPO_ENTRE_REQUEST);
                receiveData(CMD_ENVIA_ESTADO_SISTEMA, &ret, true);
                ClrLcd();
                break;
            case 0x03:
                smph.acquire();
                heating = true;
                smph.release();
                byte = 1;
                sendData(CMD_ENVIA_ESTADO_FUNCIONAMENTO, byte);
                sleepMs(TEMPO_ENTRE_REQUEST);
                receiveData(CMD_ENVIA_ESTADO_FUNCIONAMENTO, &ret, true);
                break;
            case 0x04:
                smph.acquire();
                heating = false;
                smph.release();
                byte = 0;
                sendData(CMD_ENVIA_ESTADO_FUNCIONAMENTO, byte);
                sleepMs(TEMPO_ENTRE_REQUEST);
                receiveData(CMD_ENVIA_ESTADO_FUNCIONAMENTO, &ret, true);
                break;
            case 0x05:
                timer++;
                if(!timerOn) sendData(CMD_ENVIA_TEMPORIZADOR, timer);
                else sendData(CMD_ENVIA_TEMPORIZADOR, getTimeLeft());
                sleepMs(TEMPO_ENTRE_REQUEST);
                receiveData(CMD_ENVIA_TEMPORIZADOR, &ret, true);
                break;
            case 0x06:
                timer--;
                if(!timerOn) sendData(CMD_ENVIA_TEMPORIZADOR, timer);
                else sendData(CMD_ENVIA_TEMPORIZADOR, getTimeLeft());
                sleepMs(TEMPO_ENTRE_REQUEST);
                receiveData(CMD_ENVIA_TEMPORIZADOR, &ret, true);
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
        sleepMs(TEMPO_ENTRE_REQUEST);
        receiveData(CMD_ENVIA_ESTADO_SISTEMA, &ret, true);
    }
}