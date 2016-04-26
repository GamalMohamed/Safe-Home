#ifndef PIR_H_
#define PIR_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

#define	INPUTPIN 	  PA0
#define INPUTPORT	  PINA
#define INPUTPORT_DIR DDRA


void PIR_init(void);
bool PIR_detectMotion(void);


#endif /* PIR_H_ */
