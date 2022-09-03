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
char dataFileName[] = "data.csv";

void dataHandler(){
    if(!isFileExist(dataFileName)) setHeader();

    if(turnOn){
        updateData();
        saveData();
        displayData();
    }
    sleepMs(1000);
}

void setHeader(){
    fs.open (dataFileName, std::fstream::app);
    fs << "Data e Hora,Temperatura interna,"
    << "Temperatura externa,Temperatura de referencia,"
    << "Resistor(%),Venoinha(%)" << std::endl;
    fs.close();
}

void updateData(){
    timeNow = getTimeNow();
    timeLeft = (timerOn? getTimeLeft() : timer);
}

void saveData(){
    fs.open (dataFileName, std::fstream::app);
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
        if(autoMode){
            lcdLoc(LINE1);
            typeln(recipies[menuSelection].name.c_str());
            char *line2;
            asprintf(&line2, "Temp:%02d Time:%02d", 
                recipies[menuSelection].temperature,
                recipies[menuSelection].time
            );
            lcdLoc(LINE2);
            typeln(line2);
        } else{
            lcdLoc(LINE1);
            typeln("Modo manual");
        }
    }
}

bool isFileExist(const char *fileName){
    std::ifstream infile(fileName);
    return infile.good();
}