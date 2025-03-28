#ifndef IOCONTROL_H
#define IOCONTROL_H

#include "LittleFS.h"
#include "ArduinoJson.h"
#include <vector>

#include "components/zoneOutput.h"

extern std::vector<zoneOutput> zoneOutputsList;

void controlSetup();
void controlLoop();
JsonDocument readData(fs::FS &fs, const char * path);
void createZoneOutputsList(JsonArray data, bool isPump);
void createControllerClasses(JsonDocument doc);
void formatLittleFS();
bool tempUpdated(int thermostatID, double currentTemp);
bool updateSetPoint(double newSetPoint, int zoneID);

#endif