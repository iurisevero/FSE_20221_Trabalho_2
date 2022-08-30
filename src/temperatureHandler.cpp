#include <stdio.h>

#include "temperatureHandler.hpp"
#include "pid.hpp"
#include "consts.hpp"
#include "helpers.hpp"
#include "modbus.hpp"

void heat(){
    while(run){
        if(heating){
            ssize_t rx_length;
            sendDataRequest(CMD_SOLICITA_TEMPERATURA_INTERNA);
            sleepMs(TEMPO_ENTRE_REQUEST);
            float inTemp;
            rx_length = receiveData(CMD_SOLICITA_TEMPERATURA_INTERNA, &inTemp);
            printf("%li Bytes lidos; Temperatura interna: %f\n", rx_length, inTemp);
            sleepMs(TEMPO_ENTRE_REQUEST);

            sendDataRequest(CMD_SOLICITA_TEMPERATURA_REFERENCIA);
            sleepMs(TEMPO_ENTRE_REQUEST);
            float refTemp;
            rx_length = receiveData(CMD_SOLICITA_TEMPERATURA_REFERENCIA, &refTemp);
            printf("%li Bytes lidos; Temperatura de Referencia: %f\n", rx_length, refTemp);
            sleepMs(TEMPO_ENTRE_REQUEST);

            if(!heating){
                int a;
            }

            pid_atualiza_referencia(refTemp);
            int controlSignal = pid_controle(inTemp);

            if(controlSignal > 0){
                setResistorPotency(controlSignal);
                setFanPotency(0);
            } else {
                setResistorPotency(0);
                setFanPotency(controlSignal * -1);
            }

            sleepMs(300);
        }
    }
}