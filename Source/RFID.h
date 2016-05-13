#include <SoftwareSerial.h>

/*
   TAG ID Format :
      HEX :        2 | 30 31 30 30 34 45 32 37 31 37 | 37 46                          | 3 -> Total 14 bytes (chars)
      Start byte : 2 |   10 ascii chars (10 bytes)   | 2 ascii chars (bytes) checksum | End byte : 3
   We DEFINE the TAG ID as the 10 ascii chars + 2 ascii chars of the checksum -> total 12 chars/bytes [we remove the start/end bytes]
*/

#define RX 13
#define TX 255
#define ID_length 14
#define Start_byte 2
#define End_byte 3

char c;
String Parsed_Tag_ID, Stored_Tag_ID;

SoftwareSerial UART_PORT(RX , TX);  // RX port : 13 - TX port : 255 (i.e. do not need any TX port)


bool CheckSum_Tag_ID(String Tag_ID)
{
  unsigned int b1, b2, b3, b4, b5, checksum;

  //Convert Tag_ID String into array of chars in order to use sscanf
  char charBuf[13];
  Tag_ID.toCharArray(charBuf, 13);
  sscanf(charBuf , "%2x%2x%2x%2x%2x%2x", &b1, &b2, &b3, &b4, &b5, &checksum);

  //Check now the TAG ID !
  if ( (b1 ^ b2 ^ b3 ^ b4 ^ b5) == checksum )
  {
    return true;
  }
  else
  {
    // Serial.println("Tag ID is INVALID");
    return false;
  }
}

void RFID_init()
{
  // Serial.begin(9600);

  UART_PORT.begin(9600);
}

void RFID_readID()
{
  Stored_Tag_ID = "";

  UART_PORT.listen();

  if ( UART_PORT.isListening() )
  {
    while ( UART_PORT.available() > 0 )
    {
      c = UART_PORT.read();
      Parsed_Tag_ID += c;
      if ( Parsed_Tag_ID.length() == ID_length )
      {
        if ( (Parsed_Tag_ID[0] == Start_byte) && (Parsed_Tag_ID[13] == End_byte) )
        {
          Parsed_Tag_ID = Parsed_Tag_ID.substring(1, 13);
          if ( CheckSum_Tag_ID(Parsed_Tag_ID) )
          {
            Stored_Tag_ID = Parsed_Tag_ID;
          }
        }
        Parsed_Tag_ID = "";
      }
    }
  }

  /*
    if ( Stored_Tag_ID != "" )
    {
    Serial.println("Captured Tag ID : '" +  Stored_Tag_ID + "'");
    }
  */

}
