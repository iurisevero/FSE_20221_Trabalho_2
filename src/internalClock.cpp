#include "internalClock.hpp"
#include "helpers.hpp"
#include "globalValues.hpp"
#include "temperatureHandler.hpp"
#include "timerHandler.hpp"
#include "userCommandHandler.hpp"
#include "linux_userspace.hpp"
#include "dataHandler.hpp"

int counter = 0, mod = 601;

// menu a cada 100ms, %1
// temperatura a cada 500ms, %5
// sensores BME a cada 1000ms, %10 updateSensorData()
// data handler a cada 1000ms, %10
/*
O timer só precisa ser atualizado a cada min,
já q uma variação d ms n vai afetar no valor apresentado.
O único detalhe é q a função deve ser chamada assim
que a temperatura alcançar o valor esperado
Logo, timer a cada 60000ms, %600
*/


void internalClock(){
    while(run){
        counter = (counter % mod) + 1; // Evita que counter seja 0
        if(counter % 1 == 0){
            runUserCommand();
        }

        if(counter % 5 == 0){
            heat();
        }

        if(counter %10 == 0){
            updateSensorData();
            dataHandler();
        }

        if(counter % 600 == 0){
            runTimer();
        }
        sleepMs(100);
    }

    if(somethingOn())
        sendControlSignal(0);

    printf("Finish Internal Clock (%d)\n", somethingOn());
    return;
}