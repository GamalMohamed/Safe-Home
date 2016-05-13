#include <SoftwareSerial.h>

/*
   TAG ID Format :
      HEX :        2 | 30 31 30 30 34 45 32 37 31 37 | 37 46                          | 3 -> Total 14 bytes (chars)
      Start byte : 2 |   10 ascii chars (10 bytes)   | 2 ascii chars (bytes) checksum | End byte : 3
   We DEFINE the TAG ID as the 10 ascii chars + 2 ascii chars of the checksum -> total 12 chars/bytes [we remove the start/end bytes]
*/

#define RX 13   // Pin 13
#define TX 255  // Not connected to any pin ! 
#define ID_length 14
#define Start_byte 2
#define End_byte 3
#define MAXTagsNum 4
 

void RFID_init();
String RFID_readID();
