#ifndef PCF8574_H_
#define PCF8574_H_

#include "i2c.h"


#define PCF8574_ADDRBASE (0x27) 

#define PCF8574_I2CINIT 1 

#define PCF8574_MAXDEVICES 8 //max devices, depends on address (3 bit)
#define PCF8574_MAXPINS 8   //max pin per device

static uint8_t pcf8574_pinstatus[PCF8574_MAXDEVICES];


void pcf8574_init();

/* get output status */
int8_t pcf8574_getoutput(uint8_t deviceid);

/* get output pin status */
int8_t pcf8574_getoutputpin(uint8_t deviceid, uint8_t pin);

/* set output pins */
int8_t pcf8574_setoutput(uint8_t deviceid, uint8_t data);

/* set output pins, replace actual status of a device from pinstart for pinlength with data */
int8_t pcf8574_setoutputpins(uint8_t deviceid, uint8_t pinstart, uint8_t pinlength, int8_t data);

int8_t pcf8574_setoutputpin(uint8_t deviceid, uint8_t pin, uint8_t data);

int8_t pcf8574_setoutputpinhigh(uint8_t deviceid, uint8_t pin);
int8_t pcf8574_setoutputpinlow(uint8_t deviceid, uint8_t pin);

/* get input data */
int8_t pcf8574_getinput(uint8_t deviceid);

/* get input pin (up or low) */
int8_t pcf8574_getinputpin(uint8_t deviceid, uint8_t pin);
#endif
