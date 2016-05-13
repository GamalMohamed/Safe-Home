#include "pcf8574.h"


void pcf8574_init()
{
#if PCF8574_I2CINIT == 1
  i2c_init();
#endif

  //reset the pin status
  uint8_t i = 0;
  for (i = 0; i < PCF8574_MAXDEVICES; i++)
    pcf8574_pinstatus[i] = 0;

}

int8_t pcf8574_getoutput(uint8_t deviceid)
{
  int8_t data = -1;
  if ((deviceid >= 0 && deviceid < PCF8574_MAXDEVICES))
  {
    data = pcf8574_pinstatus[deviceid];
  }
  return data;
}

int8_t pcf8574_getoutputpin(uint8_t deviceid, uint8_t pin)
{
  int8_t data = -1;
  if ((deviceid >= 0 && deviceid < PCF8574_MAXDEVICES) && (pin >= 0 && pin < PCF8574_MAXPINS))
  {
    data = pcf8574_pinstatus[deviceid];
    data = (data >> pin) & 0b00000001;
  }
  return data;
}

int8_t pcf8574_setoutput(uint8_t deviceid, uint8_t data)
{
  if ((deviceid >= 0 && deviceid < PCF8574_MAXDEVICES))
  {
    pcf8574_pinstatus[deviceid] = data;
    i2c_start(((PCF8574_ADDRBASE + deviceid) << 1) | I2C_WRITE);
    i2c_write(data);
    i2c_stop();
    return 0;
  }
  return -1;
}

int8_t pcf8574_setoutputpins(uint8_t deviceid, uint8_t pinstart, uint8_t pinlength, int8_t data)
{
  /*
     example:
     actual data is         0b01101110
     want to change              ---
     pinstart                    4
     data                        101   (pinlength 3)
     result                 0b01110110
  */
  if ((deviceid >= 0 && deviceid < PCF8574_MAXDEVICES) && (pinstart - pinlength + 1 >= 0 && pinstart - pinlength + 1 >= 0 && pinstart < PCF8574_MAXPINS && pinstart > 0 && pinlength > 0))
  {
    uint8_t b = 0;
    b = pcf8574_pinstatus[deviceid];
    uint8_t mask = ((1 << pinlength) - 1) << (pinstart - pinlength + 1);
    data <<= (pinstart - pinlength + 1);
    data &= mask;
    b &= ~(mask);
    b |= data;
    pcf8574_pinstatus[deviceid] = b;
    //update device
    i2c_start(((PCF8574_ADDRBASE + deviceid) << 1) | I2C_WRITE);
    i2c_write(b);
    i2c_stop();
    return 0;
  }
  return -1;
}

int8_t pcf8574_setoutputpin(uint8_t deviceid, uint8_t pin, uint8_t data) 
{
  if ((deviceid >= 0 && deviceid < PCF8574_MAXDEVICES) && (pin >= 0 && pin < PCF8574_MAXPINS)) 
  {
    uint8_t b = 0;
    b = pcf8574_pinstatus[deviceid];
    b = (data != 0) ? (b | (1 << pin)) : (b & ~(1 << pin));
    pcf8574_pinstatus[deviceid] = b;
    
    //update device
    i2c_start(((PCF8574_ADDRBASE + deviceid) << 1) | I2C_WRITE);
    i2c_write(b);
    i2c_stop();
    return 0;
  }
  return -1;
}

int8_t pcf8574_setoutputpinhigh(uint8_t deviceid, uint8_t pin)
{
  return pcf8574_setoutputpin(deviceid, pin, 1);
}

int8_t pcf8574_setoutputpinlow(uint8_t deviceid, uint8_t pin)
{
  return pcf8574_setoutputpin(deviceid, pin, 0);
}

int8_t pcf8574_getinput(uint8_t deviceid)
{
  int8_t data = -1;
  if ((deviceid >= 0 && deviceid < PCF8574_MAXDEVICES))
  {
    i2c_start(((PCF8574_ADDRBASE + deviceid) << 1) | I2C_READ);
    data = ~i2c_readNak();
    i2c_stop();
  }
  return data;
}

int8_t pcf8574_getinputpin(uint8_t deviceid, uint8_t pin)
{
  int8_t data = -1;
  if ((deviceid >= 0 && deviceid < PCF8574_MAXDEVICES) && (pin >= 0 && pin < PCF8574_MAXPINS))
  {
    data = pcf8574_getinput(deviceid);
    if (data != -1)
    {
      data = (data >> pin) & 0b00000001;
    }
  }
  return data;
}

