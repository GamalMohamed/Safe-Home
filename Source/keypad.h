#include "common_macros.h"

#define rows    4
#define columns 3

typedef unsigned char uint8;

const uint8 Output[rows] = {49, 48, 47, 46};
const uint8 Input[columns] = {45, 44, 43};



void KEYPAD_init()
{
  for (int i = 0; i < rows; i++)
  {
    pinMode(Output[i], OUTPUT);
  }
  for (int i = 0; i < columns; i++)
  {
    pinMode(Input[i], INPUT_PULLUP);
  }
}


uint8 KEYPAD_getkey()
{
  uint8 C = 0, R = 0;
  static bool no_press_flag = 0;
  for (int i = 0; i < columns; i++)
  {
    if (digitalRead(Input[i]) == HIGH);
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
      digitalWrite(Output[r], LOW);

    for (C = 0; C < columns; C++)
    {
      if (digitalRead(Input[C]) == HIGH)
        continue;
      else
      {
        for (R = 0; R < rows; R++)
        {
          digitalWrite(Output[R], HIGH);
          if (digitalRead(Input[C]) == HIGH)
          {
            no_press_flag = 0;

            for (int i = 0; i < rows; i++)
              digitalWrite(Output[i], LOW);

            return R * 3 + C+1;
          }
        }
      }
    }
  }

  return 99;
}





