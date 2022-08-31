#ifndef TEMPERATURE_HANDLER_HPP
#define TEMPERATURE_HANDLER_HPP

void heat();
float getInternalTemp();
float getReferenceTemp();
void sendControlSignal(int controlSignal);

#endif