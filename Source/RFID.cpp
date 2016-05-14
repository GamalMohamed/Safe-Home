#include "RFID.h"

static char c;
static String Parsed_TagID, Stored_TagID;



static SoftwareSerial UART_PORT(RX , TX);


static bool CheckSum_TagID(String TagID)
{
  unsigned int b1, b2, b3, b4, b5, checksum;

  //Convert Tag_ID String into array of chars in order to use sscanf
  char charBuf[13];
  TagID.toCharArray(charBuf, 13);
  sscanf(charBuf , "%2x%2x%2x%2x%2x%2x", &b1, &b2, &b3, &b4, &b5, &checksum);

  //Check now the TAG ID !
  if ( (b1 ^ b2 ^ b3 ^ b4 ^ b5) == checksum )
  {
    return true;
  }
  else
  {
    return false;
  }
}



void RFID_init()
{
  UART_PORT.begin(9600);
}


String RFID_readID()
{
  Stored_TagID = "";

  UART_PORT.listen();

  if ( UART_PORT.isListening() )
  {
    while ( UART_PORT.available() > 0 )
    {
      c = UART_PORT.read();
      Parsed_TagID += c;
      if ( Parsed_TagID.length() == ID_length )
      {
        if ( (Parsed_TagID[0] == Start_byte) && (Parsed_TagID[13] == End_byte) )
        {
          Parsed_TagID = Parsed_TagID.substring(1, 13);
          if ( CheckSum_TagID(Parsed_TagID) )
          {
            Stored_TagID = Parsed_TagID;
          }
        }
        Parsed_TagID = "";
      }
    }
  }


  return Stored_TagID;

}
