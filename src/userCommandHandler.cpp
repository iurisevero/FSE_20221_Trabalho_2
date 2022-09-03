#include <stdio.h>

#include "userCommandHandler.hpp"
#include "globalValues.hpp"
#include "helpers.hpp"
#include "modbus.hpp"
#include "timerHandler.hpp"
#include "i2cLCD.hpp"

void runUserCommand(){
    int userCmd;

    int retry = RETRY;
    ssize_t rx_length;
    do{
        sendDataRequest(CMD_SOLICITA_COMANDO_USUARIO);
        sleepMs(TEMPO_ENTRE_REQUEST);
        rx_length = receiveData(CMD_SOLICITA_COMANDO_USUARIO, &userCmd);
    } while(rx_length < 0 && retry--);

    if(rx_length > 0 && userCmd != 0){
        printf("Comando usuário: %#x\n", userCmd);
        handleUserInput(userCmd);
    }
}

void handleUserInput(int userCmd){
    unsigned char byte;
    int ret, retry;
    ssize_t retValue;
    if(turnOn){
        switch (userCmd){
            case 0x01:
                printf("Programa já iniciado");
                break;
            case 0x02:
                turnOn = false;
                byte = 0;
                retry = RETRY;
                do{
                    sendData(CMD_ENVIA_ESTADO_SISTEMA, byte);
                    sleepMs(TEMPO_ENTRE_REQUEST);
                    retValue = receiveData(CMD_ENVIA_ESTADO_SISTEMA, &ret, true);
                    sleepMs(TEMPO_ENTRE_REQUEST);
                } while(retValue < 0 && retry--);
                ClrLcd();
                break;
            case 0x03:
                smph.acquire();
                heating = true;
                smph.release();
                byte = 1;
                retry = RETRY;
                do{
                    sendData(CMD_ENVIA_ESTADO_FUNCIONAMENTO, byte);
                    sleepMs(TEMPO_ENTRE_REQUEST);
                    retValue = receiveData(CMD_ENVIA_ESTADO_FUNCIONAMENTO, &ret, true);
                    sleepMs(TEMPO_ENTRE_REQUEST);
                } while(retValue < 0 && retry--);
                break;
            case 0x04:
                smph.acquire();
                heating = false;
                smph.release();
                byte = 0;
                retry = RETRY;
                do{
                    sendData(CMD_ENVIA_ESTADO_FUNCIONAMENTO, byte);
                    sleepMs(TEMPO_ENTRE_REQUEST);
                    retValue = receiveData(CMD_ENVIA_ESTADO_FUNCIONAMENTO, &ret, true);
                    sleepMs(TEMPO_ENTRE_REQUEST);
                } while(retValue < 0 && retry--);
                break;
            case 0x05:
                timer++;
                retry = RETRY;
                do{
                    if(timerOn) sendData(CMD_ENVIA_TEMPORIZADOR, getTimeLeft());
                    else sendData(CMD_ENVIA_TEMPORIZADOR, timer);
                    sleepMs(TEMPO_ENTRE_REQUEST);
                    retValue = receiveData(CMD_ENVIA_TEMPORIZADOR, &ret, true);
                    sleepMs(TEMPO_ENTRE_REQUEST);
                } while(retValue < 0 && retry--);
                break;
            case 0x06:
                timer--;
                retry = RETRY;
                do{
                    if(timerOn) sendData(CMD_ENVIA_TEMPORIZADOR, getTimeLeft());
                    else sendData(CMD_ENVIA_TEMPORIZADOR, timer);
                    sleepMs(TEMPO_ENTRE_REQUEST);
                    retValue = receiveData(CMD_ENVIA_TEMPORIZADOR, &ret, true);
                    sleepMs(TEMPO_ENTRE_REQUEST);
                } while(retValue < 0 && retry--);
                break;
            case 0x07:
                if(!heating){ // Não permitir que mude a opção depois que iniciar o funcionamento
                    menuSelection = (menuSelection + 1) % MENU_SIZE;
                    autoMode = menuSelection;
                }
                break;
            default:
                printf("Comando não listado.\n");
        }
    }
    else if(userCmd == 0x01){
        turnOn = true;
        byte = 1;
        retry = RETRY;
        do{
            sendData(CMD_ENVIA_ESTADO_SISTEMA, byte);
            sleepMs(TEMPO_ENTRE_REQUEST);
            retValue = receiveData(CMD_ENVIA_ESTADO_SISTEMA, &ret, true);
            sleepMs(TEMPO_ENTRE_REQUEST);
        } while(retValue < 0 && retry--);
    }
}