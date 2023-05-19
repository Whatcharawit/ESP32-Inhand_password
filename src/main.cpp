#include "config.h"

EthernetServer server(3333);
HardwareSerial mySerial(USART1);

String recieved_number = "";
int client_id;

void setup() 
{
  mySerial.begin(115200);
  mySerial.println("Hello STM32");
  delay(100);

  pinMode(LED_LAN, OUTPUT);
  pinMode(LED_DATA, OUTPUT);
  pinMode(D0_PIN, OUTPUT);
  pinMode(D1_PIN, OUTPUT);
  digitalWrite(LED_LAN, LOW);
  digitalWrite(LED_DATA, LOW);
  delay(1000);


  Ethernet.init(PA4); // Ethernet CS pin
  //-------------------- Connect to Ethernet --------------------//
  // initialize the Ethernet device
  Ethernet.begin(mac, ip);
  Ethernet_chenk();

  //------------------ End Connect to Ethernet ------------------//


  
  delay(1000);

}

void loop() 
{
  Ethernet_chenk();
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
  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) 
  {
    mySerial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) 
    {
      digitalWrite(LED_LAN, HIGH);
      delay(100);
      digitalWrite(LED_LAN, LOW);
      delay(100);
      // delay(1); // do nothing, no point running without Ethernet hardware
    }
  }

  if (Ethernet.linkStatus() == LinkOFF) 
  {
    mySerial.println("Ethernet cable is not connected.");
    for(int i=0; i<5; i++)
    {
      digitalWrite(LED_LAN, HIGH);
      delay(50);
      digitalWrite(LED_LAN, LOW);
      delay(300);
    }
  }
  digitalWrite(LED_LAN, HIGH);

  mySerial.println(Ethernet.localIP());
  server.begin();
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
  digitalWrite(LED_DATA, HIGH);
  delay(100);
  digitalWrite(LED_DATA, LOW);
}