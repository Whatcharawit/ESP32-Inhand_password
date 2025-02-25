#include "EEPROM.h"

Preferences preferences;

void saveToEEPROM(const char* key, const String& text)
{
    preferences.begin("storage", false);  // Open storage namespace
    preferences.putString(key, text);
    preferences.end();
}

String readFromEEPROM(const char* key) 
{
    preferences.begin("storage", true);  // Open in read-only mode
    String read_data = preferences.getString(key, "No Data");
    preferences.end();
    return read_data;
}