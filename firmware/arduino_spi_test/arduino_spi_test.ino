//Arduino test sketch for reading/writing SPI to Level

#include <SPI.h>

const int MOSIpin = 11;
const int MISOpin = 12;
const int SCKpin = 13;
byte command = 0xAA;
byte rx = 0x00;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  
  SPI.setClockDivider(SPI_CLOCK_DIV8); //slow down master clock a little
  
  pinMode(MOSIpin, OUTPUT);
  pinMode(MISOpin, INPUT);
  pinMode(SCKpin, OUTPUT);
 
}

void loop(){
  int bytesToRead = 8;
  byte inByte = 0; //incoming SPI byte
  SPI.transfer(command);
  // send a value of 0 to read the first byte returned:
  rx = SPI.transfer(0x00);
  // decrement the number of bytes left to read:
  bytesToRead--;
  // if you still have another byte to read:
  if (bytesToRead > 0) {
    // shift the first byte left, then get the second byte:
    rx = rx << 8;
    inByte = SPI.transfer(0x00);
    // combine the byte you just got with the previous one:
    rx = rx | inByte;
    // decrement the number of bytes left to read:
    bytesToRead--;
  }
  
  
  Serial.println("Command sent: " + String(command));
  Serial.println("Command received: " + String(rx));
  command = command + 1;
  delay(1000);
}
