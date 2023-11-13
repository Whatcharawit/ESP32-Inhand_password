// #pragma once

#include <Arduino.h>
#include <Ethernet.h>
// #include <wizchip_conf.h>
#include <EthernetServer.h>
#include <SPI.h>
#include <HardwareSerial.h>
#include <stm32f1xx_hal.h>
#include "stm32f1xx.h"

#define LED_LAN PB8 // Note High state = LED on
#define LED_DATA PB9 // Note High state = LED on

#define D0_PIN PA3
#define D1_PIN PA2
#define WIEGAND_DELAY_MICROSECONDS 20

int eth_state;

const int WIEGAND_TOTAL_BITS = 35;
const int WIEGAND_OFFSET_BITS = 14;
const int WIEGAND_RANGE_BITS = 20;
const int WIEGAND_DELAY_US = 50;
const int adjustedValue = 170;

String OFFSET_VALUE = "00000000000000";

int recieved_data;

int bit_data[35];

// Enter a MAC address and IP address for your controller below.
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x71
};
// IPAddress ip(10, 195, 57, 93); // The IP address will be dependent on your local network:
IPAddress ip(192, 168, 1, 2); // The IP address will be dependent on your local network:
IPAddress myDns(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

void Ethernet_chenk();
void sendWiegand(int numberValue);