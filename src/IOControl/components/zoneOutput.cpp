#include "zoneOutput.h"

zoneOutput::zoneOutput(int zoneID, String zoneName, int thermostatID, double setPoint, bool isCool, bool isPump){
    this->zoneID = zoneID;
    this->zoneName = zoneName;
    this->thermostatID = thermostatID;
    this->setPoint = setPoint;
    this->isCool = isCool;
    this->isPump = isPump;
    if(isPump){
        switch(zoneID){
            case 1:
                this->_pin = ZN1_IN_PIN;
                break;
            case 2:
                this->_pin = ZN2_IN_PIN;
                break;
            case 3:
                this->_pin = ZN3_IN_PIN;
                break;
            default:
                this->_pin = 0;
                break;
        };
    }else{
        switch(zoneID){
            case 1:
                this->_pin = ZN1_OUT_PIN;
                break;
            case 2:
                this->_pin = ZN2_OUT_PIN;
                break;
            case 3:
                this->_pin = ZN3_OUT_PIN;
                break;
            case 4:
                this->_pin = ZN4_OUT_PIN;
                break;
            default:
                this->_pin = 0;
                break;
        };
    }
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);

    _isOpen = false;
    
}


void zoneOutput::printData(){
    Serial.print("Zone ID: ");
    Serial.println(zoneID);
    Serial.print("Zone Name:");
    Serial.println(zoneName);
    Serial.print("Is pump? ");
    Serial.println(isPump);
    Serial.print("Thermostat ID: ");
    Serial.println(thermostatID);
    Serial.print("Set Point: ");
    Serial.println(setPoint);
    Serial.print("Is Cool: ");
    Serial.println(isCool);
    Serial.print("pin: ");
    Serial.println(_pin);
}

bool zoneOutput::checkTemp(int thermostatID, double currentTemp){
    bool isChanged = false;
    if(this->thermostatID != thermostatID){
        return isChanged;
    }
    if(_pin == 0){
        Serial.println("No pin found. Aborting.");
        return isChanged;
    }
    if(setPoint > currentTemp){ // Too Cold
        if(isCool){
            isChanged = close();
        }else{
            isChanged = open();
        }
    }else{ // Too Hot
        if(isCool){
            isChanged = open();
        }else{
            isChanged = close();
        }
    }
    return isChanged;
    
}

bool zoneOutput::open(){
    if(!_isOpen){
        Serial.printf("Opening pin: %d, which is in %s\n", _pin, zoneName.c_str());
        // digitalWrite(_pin, HIGH);
        this->_isOpen = true;
        return _isOpen; //true
    }
    return _isOpen; //false (did not change)
}

bool zoneOutput::close(){
    if(_isOpen){
        Serial.printf("Closing pin: %d, which is in %s\n", _pin, zoneName.c_str());
        // digitalWrite(_pin, LOW);
        _isOpen = false;
        return !_isOpen; //true
    }
    return false; //false (did not change)
}

void zoneOutput::updateSetPoint(double newSetPoint, JsonDocument doc){
    // WIP : doesn't quite work...
    // update settings.json - but this will have to rewrite the entire file
    JsonArray components = doc["components"];
    for (JsonObject component : components){
        JsonArray data = component["data"];
        for (JsonObject dataItem : data) {
            if(dataItem["zoneID"] == zoneID){
                dataItem["setPoint"] = newSetPoint;
            }
        }
    }

    // TODO: check this works
    File backupFile = LittleFS.open("/settings.json", "r");
    if (!backupFile) {
        Serial.println("Failed to open file for reading");
        return;
    }
    LittleFS.remove("/settings.json"); // Delete existing file, otherwise the configuration is appended to the file
    File file = LittleFS.open("/settings.json", "w");
    if (!file) {
        Serial.println("Failed to open file for writing. ");
        return;
    }
    if (serializeJson(doc, file) == 0) {
        Serial.println(F("Failed to write to file. Reset to old settings"));
        file.print(backupFile);
        return;
    }
    file.close();
    backupFile.close();
    this->setPoint = newSetPoint;
}