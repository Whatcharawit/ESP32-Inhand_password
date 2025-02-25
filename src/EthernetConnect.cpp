#include "EthernetConnect.h"

// W5500 Pin
#define SCK 18
#define MISO 23
#define MOSI 19
#define CS 5
#define RST 16
// -------------

// Ethernet Configuration
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  // Change MAC if needed
IPAddress ip(192, 168, 2, 50);
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 255, 0);

// Server Configuration
IPAddress server(192, 168, 2, 1);
int port = 443;  // HTTPS port

int EthernetConnect()
{
    SPI.begin(SCK, MISO, MOSI, CS);
    int eth_state;

    // Reset W5500
    pinMode(RST, OUTPUT);
    digitalWrite(RST, LOW);
    delay(100);
    digitalWrite(RST, HIGH);
    delay(500);

    Ethernet.init(CS); // Ethernet CS pin

    // Initialize Ethernet
    Ethernet.begin(mac, ip, gateway, subnet);
    delay(1000);

    // Serial.print("IP Address: ");
    // Serial.println(Ethernet.localIP());

    while (Ethernet.hardwareStatus() == EthernetNoHardware) 
    {
        eth_state = 0;
        Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
        delay(1000);
        return 0;
    }

    while (Ethernet.linkStatus() == LinkOFF) 
    {
        eth_state = 0;
        Serial.println("Ethernet cable is not connected.");
        delay(1000);
        return 0;
    }

    // Initialize Ethernet
    Ethernet.begin(mac, ip, gateway, subnet);
    delay(1000);
    Serial.print("IP Address: ");
    Serial.println(Ethernet.localIP());
    eth_state = 1;
    
    return eth_state;
}