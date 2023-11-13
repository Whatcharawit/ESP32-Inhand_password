#include "config.h"

EthernetServer server(3333);
HardwareSerial mySerial(USART1);

String recieved_number = "";
int client_id;

unsigned long last_module = 0;
unsigned long last_lan = 0;

void setup() 
{
  mySerial.begin(115200);
  delay(100);

  pinMode(LED_LAN, OUTPUT);
  pinMode(LED_DATA, OUTPUT);
  pinMode(D0_PIN, OUTPUT);
  pinMode(D1_PIN, OUTPUT);
  digitalWrite(LED_LAN, HIGH);
  digitalWrite(LED_DATA, LOW);
  delay(1000);
  mySerial.println("Hello STM32");

  // Initialize the watchdog timer
  IWDG->KR = 0x5555;  // Enable access to the IWDG_PR and IWDG_RLR registers
  IWDG->PR = 4;      // Set the prescaler to achieve a 4-second timeout (256 prescaler)
  IWDG->RLR = 0xFFF;  // Set the reload value for a 4-second timeout
  IWDG->KR = 0xAAAA;  // Reload the watchdog timer
  IWDG->KR = 0xCCCC;  // Start the watchdog timer

  Ethernet.init(PA4); // Ethernet CS pin
  //-------------------- Connect to Ethernet --------------------//
  // initialize the Ethernet device
  Ethernet.begin(mac, ip, gateway, subnet);
  Ethernet_chenk();

  //------------------ End Connect to Ethernet ------------------//


  
  delay(1000);

}

void loop() 
{
  Ethernet_chenk();

  // Reset the watchdog timer to prevent a reset
  IWDG->KR = 0xAAAA;  // Reload the watchdog timer

  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // mySerial.print(c);
        recieved_number += c;
      }
    }
    client.stop();
    mySerial.println(recieved_number);
    client_id = recieved_number.toInt();
    sendWiegand(client_id);
    recieved_number = "";
  }
}

void Ethernet_chenk()
{
  last_lan = millis();
  // Check for Ethernet hardware present
  while (Ethernet.hardwareStatus() == EthernetNoHardware) 
  {
    if(millis() - last_lan >= 10000)
    {
      delay(2500);
    }
    eth_state = 0;
    mySerial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    // for(int i=0; i<12; i++)
    // {
      digitalWrite(LED_LAN, HIGH);
      delay(100);
      digitalWrite(LED_LAN, LOW);
      delay(100);
      // delay(1); // do nothing, no point running without Ethernet hardware
    // }
  }

  while (Ethernet.linkStatus() == LinkOFF) 
  {
    eth_state = 0;
    mySerial.println("Ethernet cable is not connected.");
    // for(int i=0; i<10; i++)
    // {
      digitalWrite(LED_LAN, HIGH);
      delay(50);
      digitalWrite(LED_LAN, LOW);
      delay(300);
    // }
  }
  digitalWrite(LED_LAN, HIGH);

  if(eth_state == 0)
  {
    server.begin();
    mySerial.println(Ethernet.localIP());
    eth_state = 1;
  }
}

void sendWiegand(int numberValue)
{

  String adjustString = String(adjustedValue, BIN);
  while (adjustString.length() < WIEGAND_OFFSET_BITS) {
    adjustString = "0" + adjustString;
  }
  mySerial.print("adjust string : ");
  mySerial.println(adjustString);
//   adjustString = "00000010101010";//170

  // Calculate the binary string of the adjusted value
  String binaryString = String(numberValue, BIN);

  // Pad the binary string with leading zeros to reach the range length
  while (binaryString.length() < WIEGAND_RANGE_BITS) {
    binaryString = "0" + binaryString;
  }
  mySerial.print("binary string : ");
  mySerial.println(binaryString);
//   binaryString = "00000101110100101100";//number

  // Concatenate the offset and the binary string to form the complete binary code
  String completeCode = adjustString + binaryString;


  // Calculate the parity bit (XOR of all the bits in the complete binary code)
  int parity = 0;
  for (int i = 0; i < WIEGAND_TOTAL_BITS - 1; i++) {
    parity ^= (completeCode.charAt(i) - '0');
  }

  // Add the parity bit to the end of the complete binary code
  completeCode += String(parity);
  mySerial.println(completeCode);

  // Transmit the complete binary code using the Wiegand protocol
  for (int i = 0; i < WIEGAND_TOTAL_BITS; i++) {
    int bit = completeCode.charAt(i) - '0';
    digitalWrite(bit == 1 ? D1_PIN : D0_PIN, HIGH);
    // digitalWrite(D1_PIN, bit);
    delayMicroseconds(50);
    digitalWrite(D0_PIN, LOW);
    digitalWrite(D1_PIN, LOW);
  }

  // Wait for a while before transmitting another code
  mySerial.println("send");
  mySerial.println("--------------");
//   digitalWrite(LED_DATA, HIGH);
//   delay(100);
//   digitalWrite(LED_DATA, LOW);
  for(int i=0; i<3; i++)
  {
    digitalWrite(LED_DATA, HIGH);
    delay(100);
    digitalWrite(LED_DATA, LOW);
    delay(300);
  }
}