#ifndef PIR_H_
#define PIR_H_

#include "common_macros.h"
#include "micro_config.h"

#define  INPUTPIN1     PB4  // Pin 10
#define  INPUTPIN2     PB5  // Pin 11
#define  INPUTPIN3     PB6  // Pin 12
#define  INPUTPORT     PINB
#define  INPUTPORT_DIR DDRB


void PIR_init(void);
int PIR_detectMotion(void);


#endif /* PIR_H_ */
