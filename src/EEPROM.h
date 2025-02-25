#pragma once

#include <Arduino.h>
#include <Preferences.h>

void saveToEEPROM(const char* key, const String& text);
String readFromEEPROM(const char* key);