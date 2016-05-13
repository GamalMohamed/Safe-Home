#include "keypad.h"

void KEYPAD_init()
{
  PORT_DIR = 0b00001111;
  OUTPUTPORT |= 0b01110000; //activate internal Pull-up resistance!
}


uint8 KEYPAD_getkey()
{
  uint8 C = 0, R = 0;
  static bool no_press_flag = 0;
  for (int i = 0; i < columns; i++)
  {
    if (BIT_IS_SET(INPUTPORT, i + 4));
    else
      break;

    if (i == (columns - 1))
    {
      no_press_flag = 1;
      C = 0;
      R = 0;
    }
  }

  if (no_press_flag == 1)
  {
    for (int r = 0; r < rows; r++)
    {
      CLEAR_BIT(OUTPUTPORT, r);

    }

    for (C = 0; C < columns; C++)
    {
      if (BIT_IS_SET(INPUTPORT, C + 4))
        continue;
      else
      {
        for (R = 0; R < rows; R++)
        {
          SET_BIT(OUTPUTPORT, R);

          if (BIT_IS_SET(INPUTPORT, C + 4))
          {
            no_press_flag = 0;

            for (int i = 0; i < rows; i++)
            {
              CLEAR_BIT(OUTPUTPORT, i);
            }

            return R * 3 + C;
          }
        }
      }
    }
  }

  return NOTHING;
}





