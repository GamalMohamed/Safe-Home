#include"PIR.h"

void PIR_init(void)
{
  CLEAR_BIT(INPUTPORT_DIR,INPUTPIN1); 
  CLEAR_BIT(INPUTPORT_DIR,INPUTPIN2); 
  CLEAR_BIT(INPUTPORT_DIR,INPUTPIN3); 
}


int PIR_detectMotion(void)
{
	if(BIT_IS_SET(INPUTPORT,INPUTPIN1))
	{
		return 1;
	}
	else if(BIT_IS_SET(INPUTPORT,INPUTPIN2))
	{
		return 2;
	}
  else if(BIT_IS_SET(INPUTPORT,INPUTPIN3))
  {
    return 3;
  }
  else
  {
    return NOTHING;
  }

	
}
