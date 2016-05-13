#include "common_macros.h"
#include "micro_config.h"

#define OUTPUTPORT    PORTL  //Rows
#define INPUTPORT     PINL  // Columns
#define PORT_DIR      DDRL

#define rows    4
#define columns 3

typedef unsigned char uint8;


void KEYPAD_init();
uint8 KEYPAD_getkey();





