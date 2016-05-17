/*
 * i2c.h
 *
 * Created: 5/14/2016 4:44:24 PM
 * Author: Gamal Mohamed
 */ 

#ifndef _I2C_H
#define _I2C_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <compat/twi.h>


/* defines the data direction (reading from I2C device) in i2c_start(),i2c_rep_start() */
#define I2C_READ    1

/* defines the data direction (writing to I2C device) in i2c_start(),i2c_rep_start() */
#define I2C_WRITE   0


/* initialize the I2C module interace */
void i2c_init(void);


/* Terminates the data transfer and releases the I2C bus */
void i2c_stop(void);


/* Issues a start condition and sends address and transfer direction (0: device accessible - 1: failed to access device) */
unsigned char i2c_start(unsigned char addr);


/* Issues a repeated start condition and sends address and transfer direction (0: device accessible - 1: failed to access device) */
unsigned char i2c_rep_start(unsigned char addr);


/* Issues a start condition and sends address and transfer direction. If device is busy, use ack polling to wait until device ready */
void i2c_start_wait(unsigned char addr);


/* Send one byte to I2C device (0 write successful - 1 write failed) */
unsigned char i2c_write(unsigned char data);

/* read one byte from the I2C device, request more data from device (returns byte read from I2C device) */
unsigned char i2c_readAck(void);

/* read one byte from the I2C device, read is followed by a stop condition (returns byte read from I2C device) */
unsigned char i2c_readNak(void);

/* Macro, read one byte from the I2C device (returns byte read from I2C device) */
#define i2c_read(ack)  (ack) ? i2c_readAck() : i2c_readNak();


#endif
