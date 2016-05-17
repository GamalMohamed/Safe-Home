/*
 * i2c.cpp
 *
 * Created: 5/14/2016 4:44:32 PM
 * Author: Gamal Mohamed
 */ 

#include "i2c.h"

#ifndef F_CPU
#define F_CPU 1000000UL
#endif

/* I2C clock in Hz */
#define SCL_CLOCK  10000UL


void i2c_init(void)
{
  /* initialize TWI clock: 100 kHz clock, TWPS = 0 => prescaler = 1 */

  TWSR = 0;                         /* no prescaler */
  TWBR = ((F_CPU / SCL_CLOCK) - 16) / 2; /* must be > 10 for stable operation */

}

unsigned char i2c_start(unsigned char address)
{
  uint8_t   twst;

  // send START condition
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

  // wait until transmission completed
  while (!(TWCR & (1 << TWINT)));

  // check value of TWI Status Register. Mask prescaler bits.
  twst = TW_STATUS & 0xF8;
  if ( (twst != TW_START) && (twst != TW_REP_START)) return 1;

  // send device address
  TWDR = address;
  TWCR = (1 << TWINT) | (1 << TWEN);

  // wail until transmission completed and ACK/NACK has been received
  while (!(TWCR & (1 << TWINT)));

  // check value of TWI Status Register. Mask prescaler bits.
  twst = TW_STATUS & 0xF8;
  if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1;

  return 0;

}

void i2c_start_wait(unsigned char address)
{
  uint8_t   twst;


  while ( 1 )
  {
    // send START condition
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    // wait until transmission completed
    while (!(TWCR & (1 << TWINT)));

    // check value of TWI Status Register. Mask prescaler bits.
    twst = TW_STATUS & 0xF8;
    if ( (twst != TW_START) && (twst != TW_REP_START)) continue;

    // send device address
    TWDR = address;
    TWCR = (1 << TWINT) | (1 << TWEN);

    // wail until transmission completed
    while (!(TWCR & (1 << TWINT)));

    // check value of TWI Status Register. Mask prescaler bits.
    twst = TW_STATUS & 0xF8;
    if ( (twst == TW_MT_SLA_NACK ) || (twst == TW_MR_DATA_NACK) )
    {
      /* device busy, send stop condition to terminate write operation */
      TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

      // wait until stop condition is executed and bus released
      while (TWCR & (1 << TWSTO));

      continue;
    }
    break;
  }

}

unsigned char i2c_rep_start(unsigned char address)
{
  return i2c_start( address );

}

void i2c_stop(void)
{
  /* send stop condition */
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

  // wait until stop condition is executed and bus released
  while (TWCR & (1 << TWSTO));

}

unsigned char i2c_write( unsigned char data )
{
  uint8_t   twst;

  // send data to the previously addressed device
  TWDR = data;
  TWCR = (1 << TWINT) | (1 << TWEN);

  // wait until transmission completed
  while (!(TWCR & (1 << TWINT)));

  // check value of TWI Status Register. Mask prescaler bits
  twst = TW_STATUS & 0xF8;
  if ( twst != TW_MT_DATA_ACK) return 1;
  return 0;

}

unsigned char i2c_readAck(void)
{
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
  while (!(TWCR & (1 << TWINT)));

  return TWDR;

}

unsigned char i2c_readNak(void)
{
  TWCR = (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));

  return TWDR;

}
