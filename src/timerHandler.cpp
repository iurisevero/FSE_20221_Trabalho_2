#include "timerHandler.hpp"
#include "globalValues.hpp"
#include "helpers.hpp"
#include "modbus.hpp"

#include <stdio.h>

uint64_t startTime = 0;
bool getStartTime = false;

void runTimer(){
    if(!getStartTime){
        startTime = getTimeMs();
        getStartTime = true;
    }

    if(getTimeMs() - startTime >= minToMs(timer)){
        smph.acquire();
        heating = false;
        timerOn = false;
        smph.release();
        getStartTime = false;
    }

    int retry = RETRY, retValue, ret;
    do{
        sendData(CMD_ENVIA_TEMPORIZADOR, getTimeLeft());
        sleepMs(TEMPO_ENTRE_REQUEST);
        retValue = receiveData(CMD_ENVIA_TEMPORIZADOR, &ret, true);
        sleepMs(TEMPO_ENTRE_REQUEST);
    } while(retValue < 0 && retry--);
}

uint64_t minToMs(int m){
    return m * 60000;
}

int getTimeLeft(){
    return (minToMs(timer) - (getTimeMs() - startTime)) / 60000; 
}