#include "System.h"

int main()
{
  InitializeSystem();

  while (1)
  {
    IdleSystem();

    ActiveSystem();
  }

}

