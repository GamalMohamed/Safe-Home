/*
 * Source.ino
 *
 * Created: 5/14/2016 4:37:09 PM
 * Author: Gamal Mohamed
 */ 

#include "System.h"

void setup()
{
  InitializeSystem();

}

void loop()
{
  IdleSystem();

  ActiveSystem();
}


