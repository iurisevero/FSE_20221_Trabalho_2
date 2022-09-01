#include "globalValues.hpp"

const char MATRICULA[4] = {5, 5, 1, 4};
const bool debug = true;

semaphore smph;

bool run = true, turnOn = false, heating = false, timerOn = false;
int timer = 0;
float roomTemp = 25.0f;