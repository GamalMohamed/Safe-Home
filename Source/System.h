#include "lcd.h"
#include "Keypad.h"
#include "Buzzer.h"
#include "PIR.h"
#include "RFID.h"


bool SystemActive;
String PIN;
String PAC;
String Factory_PIN;


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

String GetInput(int inputLength = 3)
{
  uint8 key = 99;
  String input = "";
  for (int i = 0; i < inputLength; i++)
  {
    while (1)
    {
      key = KEYPAD_getkey();
      if (key != 99)
        break;
    }
    _delay_ms(150);
    input += key;
  }
  return input;
}


/*************************************** System functions *********************************************/
bool DeactivateSystem()
{
  String Input = GetInput();

  if (Input == PIN)
  {
    SystemActive = false;
    return true;
  }
  else
  {
    return false;
  }

}

bool DectivateRequest()
{
  DisplayMessage("Deactivation..");
  _delay_ms(1000);

  ClearScreen();
  DisplayMessage("Enter PIN\n MAX trials:4");

  for (int i = 4; i > 0; i--)
  {
    if (DeactivateSystem())
    {
      ClearScreen();
      DisplayMessage("Deactivated!");

      _delay_ms(1000);
      return true;
    }
    else
    {
      ClearScreen();
      DisplayMessage("Wrong PIN!!");

      _delay_ms(1000);

      ClearScreen();
      DisplayMessage("Enter PIN again");
    }
  }

  return false;
}

void ResetPIN()
{
  ClearScreen();
  DisplayMessage("PIN Reset..");
  _delay_ms(1000);

  ClearScreen();
  DisplayMessage("Enter old PIN");
  String Input = GetInput();

  if (Input == PIN)
  {
    ClearScreen();
    DisplayMessage("Enter new PIN");
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

bool FactorySettings()
{
  ClearScreen();
  DisplayMessage("Restore Factory\n settings..");
  _delay_ms(1000);

  ClearScreen();
  DisplayMessage("Enter PAC");
  String Input = GetInput();

  if (Input == PAC)
  {
    PIN = Factory_PIN;
    SystemActive = false;
    ClearScreen();
    DisplayMessage("System\n  Restored!");
    _delay_ms(1000);
    return true;
  }
  else
  {
    ClearScreen();
    DisplayMessage("Invalid PAC\n Try again..");
    _delay_ms(1000);

    return false;
  }



}

void DetectMotion()
{
  bool intrusion = false;
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
      break;

    case 3:
      ClearScreen();
      DisplayMessage("RM2 Intrusion");
      intrusion = true;
      break;

    default:
      intrusion = false;
  }

  // Activate Buzzer!
  if (intrusion)
  {
    //while (1)
    {
      Alert(300, 160);
      _delay_ms(150);

      /*
        if (ValidRFID())
        {
          ClearScreen();
          DisplayMessage("System AC");
          return;
        }
      */
    }
  }

}

void InitializeSystem()
{
  //Initilaize all devices
  LCD_init();
  KEYPAD_init();
  PIR_init();
  Buzzer_init();

  SystemActive = false; //System by default is inactive

  //Manufacture's settings
  Factory_PIN = "123";
  PAC = "999";
  PIN = Factory_PIN;

  // Welcome Message!
  DisplayMessage("Welcome to Safe\n Home system!");
  _delay_ms(2000);
}

void IdleSystem()
{
  ClearScreen();
  while (1)
  {
    DisplayMessage("Press 1 to\n activate system");

    uint8 key = GetKey();
    if (key == 1) //1 is pressed
    {
      ClearScreen();
      DisplayMessage("Enter PIN");

      if (PIN == GetInput())
      {
        SystemActive = true;
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
    else if (key == 10)  //'*'
    {
      ResetPIN();
      ClearScreen();
    }
    else if (key == 11) // '0'
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
        0. Detect Motion
        1. Deactivate system  (#)
        2. Factory settings   (0)
        3. Reset PIN          (*)
  */

  ClearScreen();
  DisplayMessage("System AC");
  while (1)
  {
    DetectMotion();

    uint8 key = GetKey();
    if ( key != 99 )
    {
      if (key == 12)      //'#'
      {
        ClearScreen();
        if (!DectivateRequest())
        {
          ClearScreen();
          DisplayMessage("System Locked!!\n  Use PAC..");
          _delay_ms(1000);

          ClearScreen();
          DisplayMessage("System AC");
        }
        else
        {
          ClearScreen();
          DisplayMessage("System AC");
          return;
        }
      }
      else if (key == 11) // '0'
      {
        if (FactorySettings())
        {
          ClearScreen();
          DisplayMessage("System AC");
          return;
        }
        else
        {
          ClearScreen();
          DisplayMessage("System AC");
        }
      }
    }

    
  }

}
