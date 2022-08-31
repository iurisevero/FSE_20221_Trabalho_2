#include "timerHandler.hpp"
#include "globalValues.hpp"
#include "helpers.hpp"
#include "modbus.hpp"

uint64_t startTime = 0;

void runTimer(){
    bool getStartTime = false;
    while(run){
        if(timerOn){
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

            sendData(CMD_ENVIA_TEMPORIZADOR, getTimeLeft());

            sleepMs(200);
        } else {
            sleepMs(600);
        }
    }
    printf("TimerHandler finished\n");
    return;
}

uint64_t minToMs(int m){
    return m * 60000;
}

int getTimeLeft(){
    return (minToMs(timer) - (getTimeMs() - startTime)) / 60000; 
}