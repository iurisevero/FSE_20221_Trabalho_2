#include <stdio.h>
#include <cmath>

#include "temperatureHandler.hpp"
#include "timerHandler.hpp"
#include "pid.hpp"
#include "globalValues.hpp"
#include "helpers.hpp"
#include "modbus.hpp"

void heat(){
    const float eps = 0.005f;
    if(heating){
        inTemp = getInternalTemp();
        // Se tiver no modo automático, bloqueia a alteração da temperatura de referência
        refTemp = (autoMode? recipies[menuSelection].temperature : getReferenceTemp());

        // Se a diferença entre a temperatura interna e a de referência
        // for menor que 0,5% da temperatura de referência, o timer é acionado
        if(!timerOn && fabs(inTemp - refTemp) < (refTemp * eps)){
            smph.acquire();
            timerOn = true;
            smph.release();
            runTimer();
        }

        pid_atualiza_referencia(refTemp);
        int controlSignal = pid_controle(inTemp);
        sendControlSignal(controlSignal);
    } else if(turnOn && inTemp > roomTemp){
        inTemp = getInternalTemp();
        pid_atualiza_referencia(roomTemp);
        int controlSignal = pid_controle(inTemp);
        sendControlSignal(controlSignal);
    } else {
        if(somethingOn())
            sendControlSignal(0);
    }
}

float getInternalTemp(){
    ssize_t rx_length;
    float _inTemp;
    int retry = RETRY;
    do{
        sendDataRequest(CMD_SOLICITA_TEMPERATURA_INTERNA);
        sleepMs(TEMPO_ENTRE_REQUEST);
        rx_length = receiveData(CMD_SOLICITA_TEMPERATURA_INTERNA, &_inTemp);
        if(debug) printf("%ld Bytes lidos; Temperatura interna: %f\n", rx_length, _inTemp);
        sleepMs(TEMPO_ENTRE_REQUEST);
    } while(rx_length < 0 && retry--);

    return (rx_length < 0? inTemp : _inTemp);
}

float getReferenceTemp(){
    ssize_t rx_length;
    float _refTemp;
    int retry = RETRY;
    do{
        sendDataRequest(CMD_SOLICITA_TEMPERATURA_REFERENCIA);
        sleepMs(TEMPO_ENTRE_REQUEST);
        rx_length = receiveData(CMD_SOLICITA_TEMPERATURA_REFERENCIA, &_refTemp);
        if(debug) printf("%ld Bytes lidos; Temperatura de Referencia: %f\n", rx_length, _refTemp);
        sleepMs(TEMPO_ENTRE_REQUEST);
    } while(rx_length < 0 && retry--);

    return (rx_length < 0? refTemp : _refTemp);
}

void sendControlSignal(int controlSignal){
    sendData(CMD_ENVIA_SINAL_CONTROLE, controlSignal);
    if(controlSignal > 0){
        setResistorPotency(controlSignal);
        setFanPotency(0);
        resistorPercent = controlSignal;
        fanPercent = 0;
    } else {
        setResistorPotency(0);
        setFanPotency(controlSignal * -1);
        resistorPercent = 0;
        fanPercent = controlSignal * -1;
    }
}