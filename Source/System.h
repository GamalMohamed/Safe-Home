#include "lcd.h"
#include "keypad.h"
#include "Buzzer.h"
#include "PIR.h"
#include "RFID.h"


bool SystemActive;
String PIN;
String PAC;
String Factory_PIN;
String MasterCardID;
String StoredTags[MAXTagsNum];



/*************************************** Utility functions *********************************************/
inline void ClearScreen()
{
  LCD_clearScreen();
}

inline void Home()
{
  LCD_home();
}

inline void DisplayMessage(const char* msg)
{
  LCD_puts(msg);
}

inline uint8 GetKey()
{
  return KEYPAD_getkey();
}

String GetInput(int inputLength = 6)
{
  uint8 key = NOTHING;
  String input = "";

  for (int i = 0; i < inputLength; i++)
  {
    while (1)
    {
      key = KEYPAD_getkey();
      if (key != NOTHING)
      {
        DisplayMessage("*");
        break;
      }
    }
    input += key;
    _delay_ms(500);
  }
  //DisplayMessage(input.c_str());
  //_delay_ms(1000);
  return input;
}


/*************************************** System functions *********************************************/

/* Returns true only if ID provided is found in the StoredTags array and thus returns its no. */
bool FindTag(String ID, int &Tag_num)
{
  for (int i = 0; i < MAXTagsNum; i++)
  {
    if (ID == StoredTags[i])
    {
      Tag_num = i + 1;
      return true;
    }
  }

  return false;
}

/* Returns:  0 => No ID provided | -1 => Invalid ID  | 11 => MasterCard | Tag no. => Valid Tag */
int ValidID()
{
  int Tag_num = -1;
  String ID = RFID_readID();

  if (ID != "")
  {
    if (ID == MasterCardID)
    {
      return 11;
    }
    else if (FindTag(ID, Tag_num))
    {
      return Tag_num;
    }
    else
    {
      return -1; // Invalid ID !!
    }
  }
  else
  {
    return 0; // No ID provided !
  }
}

bool IDRequest()
{
  int ID = ValidID();
  if (ID != 0)
  {
    if (ID == -1)
    {
      ClearScreen();
      DisplayMessage("INVALID ID !!");
      _delay_ms(1000);

      ClearScreen();
      DisplayMessage(" System ACTIVE");
      return false;
    }
    else
    {
      return true;
    }
  }
  else
  {
    return false;
  }
}

void DeactivateSystem()
{
  SystemActive = false;

  ClearScreen();
  DisplayMessage("System\n Deactivated!");
  _delay_ms(1000);
}

void ResetPIN()
{
  ClearScreen();
  DisplayMessage("PIN Reset..");
  _delay_ms(1000);

  ClearScreen();
  DisplayMessage("Enter old PIN\n");
  String Input = GetInput();

  if (Input == PIN)
  {
    ClearScreen();
    DisplayMessage("Enter new PIN\n");
    PIN = GetInput();

    ClearScreen();
    DisplayMessage("PIN Reset!");
    _delay_ms(1000);
  }
  else
  {
    ClearScreen();
    DisplayMessage("Invalid PIN\n Try again..");
    _delay_ms(1000);
  }
}

void FactorySettings()
{
  ClearScreen();
  DisplayMessage("Restore Factory\n settings..");
  _delay_ms(1000);

  ClearScreen();
  DisplayMessage("Enter PAC\n");
  String Input = GetInput();

  if (Input == PAC)
  {
    PIN = Factory_PIN;
    SystemActive = false;

    ClearScreen();
    DisplayMessage("System\n  Restored!");
    _delay_ms(1000);
  }
  else
  {
    ClearScreen();
    DisplayMessage("Invalid PAC\n Try again..");
    _delay_ms(1000);
  }
}

void IntrusionAlert(bool intrusion, bool room)
{
  // Activate Buzzer!
  if (intrusion)
  {
    while (1)
    {
      if (!room)
      {
        if (IDRequest())
        {
          DeactivateSystem();
          return;
        }
      }
      Alert(300, 160);
      _delay_ms(150);
    }
  }
}

void DetectMotion()
{
  bool intrusion = false; // to detect intrusion. 
  bool room = false;
  switch (PIR_detectMotion())
  {
    case 1:
      ClearScreen();
      DisplayMessage("Hall Intrusion");
      intrusion = true;
      break;

    case 2:
      ClearScreen();
      DisplayMessage("RM1 Intrusion");
      intrusion = true;
      room = true;
      break;

    case 3:
      ClearScreen();
      DisplayMessage("RM2 Intrusion");
      intrusion = true;
      room = true;
      break;

    default:
      intrusion = false;
  }

  IntrusionAlert(intrusion, room);
} 

void InitializeSystem()
{
  // Initilaize all devices
  LCD_init();
  KEYPAD_init();
  PIR_init();
  Buzzer_init();
  RFID_init();

  SystemActive = false; // System by default is inactive

  // Manufacture's settings
  Factory_PIN = "123456";
  PAC = "333999";
  MasterCardID = "3800700780CF";
  StoredTags[0] = "01002BC8EB09";

  PIN = Factory_PIN;


  // Welcome Message!
  DisplayMessage("Welcome to Safe\n  Home system!");
  _delay_ms(2000);
}

void IdleSystem()
{
  /*
     Options:
        1. Activate system    (1)
        2. Factory settings   (#)
        3. Reset PIN          (*)
  */

  ClearScreen();
  while (1)
  {
    DisplayMessage("Press 1 to\n activate system");

    uint8 key = GetKey();
    if (key == 1)  // '1'
    {
      _delay_ms(500);
      ClearScreen();
      DisplayMessage("Enter PIN\n");

      if (PIN == GetInput())
      {
        SystemActive = true;
        IDRequest();
        return;
      }
      else
      {
        ClearScreen();
        DisplayMessage("Wrong PIN");
        _delay_ms(1000);

        ClearScreen();
      }
    }
    else if (key == 10)  // '*'
    {
      ResetPIN();
      ClearScreen();
    }
    else if (key == 12) // '#'
    {
      FactorySettings();
      ClearScreen();
    }
  }
}

void ActiveSystem()
{
  /*
     Options:
        1. Detect Motion
        2. Deactivate system  (ID)
  */

  ClearScreen();
  DisplayMessage(" System ACTIVE");
  while (1)
  {
    DetectMotion();

    if (SystemActive)
    {
      if (IDRequest())
      {
        DeactivateSystem();
        _delay_ms(1000);
        return;
      }
    }
    else
    {
      return;
    }
  }
}
