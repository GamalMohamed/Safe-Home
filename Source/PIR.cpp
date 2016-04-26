#include"PIR.h"

void PIR_init(void)
{
  CLEAR_BIT(INPUTPORT_DIR,INPUTPIN); //make PE4 as input pin
}


bool PIR_detectMotion(void)
{
	if(BIT_IS_SET(INPUTPORT,INPUTPIN))
	{
		return true;
	}
	else
	{
		return false;
	}
	
}
