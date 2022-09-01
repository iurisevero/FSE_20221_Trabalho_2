#include "dataHandler.hpp"
#include "globalValues.hpp"
#include "linux_userspace.hpp"
#include "temperatureHandler.hpp"
#include "timerHandler.hpp"
#include "helpers.hpp"
#include "i2cLCD.hpp"

#include <iostream>
#include <fstream>
#include <string>

std::fstream fs;
std::string timeNow;
int timeLeft;
float inTemp, refTemp;

void dataHandler(){
    fs.open ("data.csv", std::fstream::app);
    fs << "Data e Hora,Temperatura interna,"
    << "Temperatura externa,Temperatura de referencia,"
    << "Resistor(%),Venoinha(%)" << std::endl;
    fs.close();
    while(run){
        if(turnOn){
            updateData();
            saveData();
            displayData();
        }
        sleepMs(1000);
    }
}

void updateData(){
    timeNow = getTimeNow();
    timeLeft = getTimeLeft();
    inTemp = getInternalTemp();
    refTemp = getReferenceTemp();
}

void saveData(){
    fs.open ("data.csv", std::fstream::app);
    fs << timeNow << ","
    << inTemp << ","
    << roomTemp << ","
    << refTemp << ","
    << resistorPercent << ","
    << fanPercent << ","
    << std::endl;
    fs.close();
}

void displayData(){
    if(heating){
        ClrLcd();
        char *line1, *line2;
        asprintf(&line1, "Temp: %2f | %2f", inTemp, refTemp);
        lcdLoc(LINE1);
        typeln(line1);
        asprintf(&line2, "Time left: %d", timeLeft);
        lcdLoc(LINE2);
        typeln(line2);
    }else{
        ClrLcd();
        lcdLoc(LINE1);
        typeln("Menu");
    }
}