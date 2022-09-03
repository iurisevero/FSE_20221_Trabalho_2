#include "globalValues.hpp"

const char MATRICULA[4] = {5, 5, 1, 4};
const bool debug = false;

semaphore smph;

bool run = true, turnOn = false, heating = false, timerOn = false, autoMode = false;
int timer = 0, fanPercent = 0, resistorPercent = 0, menuSelection = 0;
float roomTemp = 30.0f, inTemp = 0, refTemp = 0;

preDefinedRecepies recipies[6] = {
    {"Menu",             0,  0},
    {"Frango",          30, 40},
    {"Carne",           45, 60},
    {"Peixe",           25, 50},
    {"Batata",          15, 20},
    {"Pao de Queijo",   15, 30},
};