#include <stdio.h>
#include <cmath>

#include "temperatureHandler.hpp"
#include "pid.hpp"
#include "globalValues.hpp"
#include "helpers.hpp"
#include "modbus.hpp"

void heat(){
    const float eps = 0.005f;
    float inTemp, refTemp;
    while(run){
        if(heating){
            inTemp = getInternalTemp();
            refTemp = getReferenceTemp();            

            // Se a diferença entre a temperatura interna e a de referência
            // for menor que 0,5% da temperatura de referência, o timer é acionado
            if(!timerOn && fabs(inTemp - refTemp) < (refTemp * eps)){
                smph.acquire();
                timerOn = true;
                smph.release();
            }

            pid_atualiza_referencia(refTemp);
            int controlSignal = pid_controle(inTemp);
            sendControlSignal(controlSignal);

            sleepMs(300);
        } else if(turnOn && inTemp > roomTemp){
            inTemp = getInternalTemp();
            pid_atualiza_referencia(roomTemp);
            int controlSignal = pid_controle(inTemp);
            sendControlSignal(controlSignal);
            sleepMs(400);
        } else {
            if(somethingOn())
                sendControlSignal(0);
            sleepMs(500);
        }
    }

    if(somethingOn())
        sendControlSignal(0);
    
    printf("TemperatureHandler finished (%d)\n", somethingOn());
    return;
}

float getInternalTemp(){
    ssize_t rx_length;
    sendDataRequest(CMD_SOLICITA_TEMPERATURA_INTERNA);
    sleepMs(TEMPO_ENTRE_REQUEST);
    float inTemp;
    rx_length = receiveData(CMD_SOLICITA_TEMPERATURA_INTERNA, &inTemp);
    printf("%d Bytes lidos; Temperatura interna: %f\n", rx_length, inTemp);
    sleepMs(TEMPO_ENTRE_REQUEST);
    return inTemp;
}

float getReferenceTemp(){
    ssize_t rx_length;
    sendDataRequest(CMD_SOLICITA_TEMPERATURA_REFERENCIA);
    sleepMs(TEMPO_ENTRE_REQUEST);
    float refTemp;
    rx_length = receiveData(CMD_SOLICITA_TEMPERATURA_REFERENCIA, &refTemp);
    printf("%d Bytes lidos; Temperatura de Referencia: %f\n", rx_length, refTemp);
    sleepMs(TEMPO_ENTRE_REQUEST);
    return refTemp;
}

void sendControlSignal(int controlSignal){
    sendData(CMD_ENVIA_SINAL_CONTROLE, controlSignal);
    if(controlSignal > 0){
        setResistorPotency(controlSignal);
        setFanPotency(0);
    } else {
        setResistorPotency(0);
        setFanPotency(controlSignal * -1);
    }
}