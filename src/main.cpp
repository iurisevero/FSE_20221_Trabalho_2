#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <thread>
#include <csignal>

#include "globalValues.hpp"
#include "helpers.hpp"
#include "uart.hpp"
#include "crc16.hpp"
#include "modbus.hpp"
#include "pid.hpp"
#include "i2cLCD.hpp"

#include "temperatureHandler.hpp"
#include "timerHandler.hpp"
#include "userCommandHandler.hpp"
#include "linux_userspace.hpp"

void signalHandler(int signum){
    printf("Interrupt signal (%d) received.\n", signum);
    printf("Stopping program.... It may take some seconds while the threads are finished\n");
    run = false;
}

int setPins(){
    if(wiringPiSetup() == -1)
        return -1;

    pinMode(RESISTOR, OUTPUT);
    pinMode(VENTOINHA, OUTPUT);
    softPwmCreate(RESISTOR, 0, 100);
    softPwmCreate(VENTOINHA, 0, 100);
    return 0;
}

void reset(){
    sendControlSignal(0);
    unsigned char byte = 0;
    sendData(CMD_ENVIA_ESTADO_SISTEMA, byte);
    sendData(CMD_ENVIA_ESTADO_FUNCIONAMENTO, byte);
    sendData(CMD_ENVIA_TEMPORIZADOR, 0);
    ClrLcd();
}

int main(int argc, const char * argv[]) {
    int wiringPiFd, wiringPiI2CFd, uartFd, i2cFd;
    if((wiringPiFd = setPins()) == -1)
        return -1;

    wiringPiI2CFd = wiringPiI2CSetup(I2C_ADDR);
    lcd_init(wiringPiI2CFd);

    char uartPath[] = "/dev/serial0";
    if((uartFd = Uart::openUart(uartPath)) == -1)
        return -1;
    
    char i2cPath[] = "/dev/i2c-1";
    if ((i2cFd = open(i2cPath, O_RDWR)) < 0){
        fprintf(stderr, "Failed to open the i2c bus %s\n", i2cPath);
        exit(1);
    }

    pid_configura_constantes(30.0, 0.2, 400.0);

    signal(SIGINT, signalHandler); 
    smph.release();

    reset();

    std::thread runUserCommandHandler(runUserCommand);
    std::thread runTimerHandler(runTimer);
    std::thread runTemperatureHandler(heat);
    std::thread runSensors(runBME280Sensors, i2cFd);
    runUserCommandHandler.join();
    runTimerHandler.join();
    runTemperatureHandler.join();
    runSensors.join();

    close(wiringPiFd);
    close(wiringPiI2CFd);
    close(uartFd);
    close(i2cFd);
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