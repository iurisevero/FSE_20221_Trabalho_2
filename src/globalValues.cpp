#include "globalValues.hpp"

const char MATRICULA[4] = {5, 5, 1, 4};
const bool debug = true;

semaphore smph;

bool run = true, turnOn = false, heating = false, timerOn = false;
int timer = 0, fanPercent = 0, resistorPercent = 0;
float roomTemp = 30.0f, inTemp = 0, refTemp = 0;
