#ifndef IOCONTROL_H
#define IOCONTROL_H

/*
  IOControl.h - Library for Input/Output Control for climate systems.
  Created by Megan Veldhuis, April 2, 2025.
*/

#include "LittleFS.h"
#include "ArduinoJson.h"
#include <vector>

#include "components/zoneOutput.h"
#include "components/endSwitch.h"
#include "components/ADCOutput.h"
#include "components/thermistorPort.h"

extern std::vector<zoneOutput> zoneOutputsList;
extern std::vector<thermistorPort> thermistorPortsList;
extern endSwitch globalEndSwitch;
extern ADCOutput globalADCOutput;
extern String globalControllerType;

void controlSetup();
JsonDocument readData(fs::FS &fs, const char * path);
void initZoneOutputs(JsonArray data, bool isPump);
void createControllerClasses(JsonDocument doc);
void formatLittleFS();
void tempUpdated(int thermostatID, float currentTemp);
bool updateSetPoint(float newSetPoint, int zoneID, String fanCoilName);

void updateControls();
bool isZoneOpen(int zoneID);
bool isADCOn();
float getThermistorTemp(String thermistorName);

#endif