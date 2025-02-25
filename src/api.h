#pragma once

#include <Arduino.h>
#include <ESP_SSLClient.h>
#include <ArduinoJson.h>

#include "EthernetConnect.h"

String login(String authen);
String ChangePasswd(String web_session, String current_passwd, String new_passwd);