#include "lcd.h"

#define LCD_e_delay()   __asm__ __volatile__( "rjmp 1f\n 1:" );
#define LCD_e_toggle()  toggle_e()

/* delay for a minimum of <us> microseconds, the number of loops is calculated at compile-time from MCU clock frequency */
#define delay(us)  _delayFourCycles( ( ( 1*(F_CPU/4000) )*us)/1000 )


#if LCD_LINES==1
#define LCD_FUNCTION_DEFAULT    LCD_FUNCTION_4BIT_1LINE
#else
#define LCD_FUNCTION_DEFAULT    LCD_FUNCTION_4BIT_2LINES
#endif

static uint8_t dataport = 0;

/* delay loop for small accurate delays: 16-bit counter, 4 cycles/loop */
static inline void _delayFourCycles(unsigned int __count)
{
  if ( __count == 0 )
    __asm__ __volatile__( "rjmp 1f\n 1:" );    // 2 cycles
  else
    __asm__ __volatile__ (
      "1: sbiw %0,1" "\n\t"
      "brne 1b"                              // 4 cycles/loop
      : "=w" (__count)
      : "0" (__count)
    );
}

/* toggle Enable Pin to initiate write */
static void toggle_e(void)
{
  pcf8574_setoutputpinhigh(LCD_PCF8574_DEVICEID, LCD_E_PIN);
  //LCD_e_delay();
  _delay_ms(1);
  pcf8574_setoutputpinlow(LCD_PCF8574_DEVICEID, LCD_E_PIN);
}


/* Low-level function to write byte to LCD controller
  Input:    data    byte to write to LCD
             rs     1: write data
                    0: write instruction */
static void LCD_write(uint8_t data, uint8_t rs)
{
  if (rs) /* write data        (RS=1, RW=0) */
    dataport |= _BV(LCD_RS_PIN);
  else /* write instruction (RS=0, RW=0) */
    dataport &= ~_BV(LCD_RS_PIN);
  dataport &= ~_BV(LCD_RW_PIN);
  pcf8574_setoutput(LCD_PCF8574_DEVICEID, dataport);

  /* output high nibble first */
  dataport &= ~_BV(LCD_DATA3_PIN);
  dataport &= ~_BV(LCD_DATA2_PIN);
  dataport &= ~_BV(LCD_DATA1_PIN);
  dataport &= ~_BV(LCD_DATA0_PIN);
  if (data & 0x80) dataport |= _BV(LCD_DATA3_PIN);
  if (data & 0x40) dataport |= _BV(LCD_DATA2_PIN);
  if (data & 0x20) dataport |= _BV(LCD_DATA1_PIN);
  if (data & 0x10) dataport |= _BV(LCD_DATA0_PIN);
  pcf8574_setoutput(LCD_PCF8574_DEVICEID, dataport);
  LCD_e_toggle();

  /* output low nibble */
  dataport &= ~_BV(LCD_DATA3_PIN);
  dataport &= ~_BV(LCD_DATA2_PIN);
  dataport &= ~_BV(LCD_DATA1_PIN);
  dataport &= ~_BV(LCD_DATA0_PIN);
  if (data & 0x08) dataport |= _BV(LCD_DATA3_PIN);
  if (data & 0x04) dataport |= _BV(LCD_DATA2_PIN);
  if (data & 0x02) dataport |= _BV(LCD_DATA1_PIN);
  if (data & 0x01) dataport |= _BV(LCD_DATA0_PIN);
  pcf8574_setoutput(LCD_PCF8574_DEVICEID, dataport);
  LCD_e_toggle();

  /* all data pins high (inactive) */
  dataport |= _BV(LCD_DATA0_PIN);
  dataport |= _BV(LCD_DATA1_PIN);
  dataport |= _BV(LCD_DATA2_PIN);
  dataport |= _BV(LCD_DATA3_PIN);
  pcf8574_setoutput(LCD_PCF8574_DEVICEID, dataport);
}

/* Low-level function to read byte from LCD controller
   Input:      rs    1: read data
                     0: read busy flag / address counter */
static uint8_t LCD_read(uint8_t rs)
{
  uint8_t data;

  if (rs) /* write data        (RS=1, RW=0) */
    dataport |= _BV(LCD_RS_PIN);
  else /* write instruction (RS=0, RW=0) */
    dataport &= ~_BV(LCD_RS_PIN);
  dataport |= _BV(LCD_RW_PIN);
  pcf8574_setoutput(LCD_PCF8574_DEVICEID, dataport);

  pcf8574_setoutputpinhigh(LCD_PCF8574_DEVICEID, LCD_E_PIN);
  LCD_e_delay();
  data = pcf8574_getoutputpin(LCD_PCF8574_DEVICEID, LCD_DATA0_PIN) << 4;     /* read high nibble first */
  pcf8574_setoutputpinlow(LCD_PCF8574_DEVICEID, LCD_E_PIN);

  LCD_e_delay();                       /* Enable 500ns low       */

  pcf8574_setoutputpinhigh(LCD_PCF8574_DEVICEID, LCD_E_PIN);
  LCD_e_delay();
  data |= pcf8574_getoutputpin(LCD_PCF8574_DEVICEID, LCD_DATA0_PIN) & 0x0F;   /* read low nibble        */
  pcf8574_setoutputpinlow(LCD_PCF8574_DEVICEID, LCD_E_PIN);

  return data;
}

/* loops while lcd is busy, returns address counter */
static uint8_t LCD_waitbusy(void)
{
  register uint8_t c;

  /* wait until busy flag is cleared */
  while ( (c = LCD_read(0)) & (1 << LCD_BUSY)) {}

  /* the address counter is updated 4us after the busy flag is cleared */
  delay(2);

  /* now read the address counter */
  return (LCD_read(0));  // return address counter

}

/* Move cursor to the start of next line or to the first line if the cursor is already on the last line */
static inline void LCD_newline(uint8_t pos)
{
  register uint8_t addressCounter;


#if LCD_LINES==1
  addressCounter = 0;
#endif
#if LCD_LINES==2
  if ( pos < (LCD_START_LINE2) )
    addressCounter = LCD_START_LINE2;
  else
    addressCounter = LCD_START_LINE1;
#endif
#if LCD_LINES==4
  if ( pos < LCD_START_LINE3 )
    addressCounter = LCD_START_LINE2;
  else if ( (pos >= LCD_START_LINE2) && (pos < LCD_START_LINE4) )
    addressCounter = LCD_START_LINE3;
  else if ( (pos >= LCD_START_LINE3) && (pos < LCD_START_LINE2) )
    addressCounter = LCD_START_LINE4;
  else
    addressCounter = LCD_START_LINE1;
#endif
  LCD_command((1 << LCD_DDRAM) + addressCounter);

}



void LCD_init(uint8_t dispAttr)
{
#if LCD_PCF8574_INIT == 1
  pcf8574_init();
#endif

  dataport = 0;
  pcf8574_setoutput(LCD_PCF8574_DEVICEID, dataport);

  _delay_ms(10);        /* wait 16ms or more after power-on       */

  /* initial write to lcd is 8bit */
  dataport |= _BV(LCD_DATA1_PIN);  // _BV(LCD_FUNCTION)>>4;
  dataport |= _BV(LCD_DATA0_PIN);  // _BV(LCD_FUNCTION_8BIT)>>4;
  pcf8574_setoutput(LCD_PCF8574_DEVICEID, dataport);

  LCD_e_toggle();
  _delay_ms(1);         /* delay, busy flag can't be checked here */

  /* repeat last command */
  LCD_e_toggle();
  _delay_ms(1);           /* delay, busy flag can't be checked here */

  /* repeat last command a third time */
  LCD_e_toggle();
  _delay_ms(1);           /* delay, busy flag can't be checked here */

  /* now configure for 4bit mode */
  dataport &= ~_BV(LCD_DATA0_PIN);
  pcf8574_setoutput(LCD_PCF8574_DEVICEID, dataport);
  LCD_e_toggle();
  _delay_ms(1);           /* some displays need this additional delay */

  /* from now the LCD only accepts 4 bit I/O, we can use lcd_command() */

  LCD_command(LCD_FUNCTION_DEFAULT);      /* function set: display lines  */

  LCD_command(LCD_DISP_OFF);              /* display off                  */
  LCD_clearScreen();                           /* display clear                */
  LCD_command(LCD_MODE_DEFAULT);          /* set entry mode               */
  LCD_command(dispAttr);                  /* display/cursor control       */
  LCD_led(0);
}

void LCD_command(uint8_t cmd)
{
  LCD_waitbusy();
  LCD_write(cmd, 0);
}

void LCD_data(uint8_t data)
{
  LCD_waitbusy();
  LCD_write(data, 1);
}

void LCD_gotoxy(uint8_t x, uint8_t y)
{
#if LCD_LINES==1
  LCD_command((1 << LCD_DDRAM) + LCD_START_LINE1 + x);
#endif
#if LCD_LINES==2
  if ( y == 0 )
    LCD_command((1 << LCD_DDRAM) + LCD_START_LINE1 + x);
  else
    LCD_command((1 << LCD_DDRAM) + LCD_START_LINE2 + x);
#endif
#if LCD_LINES==4
  if ( y == 0 )
    LCD_command((1 << LCD_DDRAM) + LCD_START_LINE1 + x);
  else if ( y == 1)
    LCD_command((1 << LCD_DDRAM) + LCD_START_LINE2 + x);
  else if ( y == 2)
    LCD_command((1 << LCD_DDRAM) + LCD_START_LINE3 + x);
  else /* y==3 */
    LCD_command((1 << LCD_DDRAM) + LCD_START_LINE4 + x);
#endif

}

int LCD_getxy(void)
{
  return LCD_waitbusy();
}

void LCD_clearScreen(void)
{
  LCD_command(1 << LCD_CLR);
}

void LCD_led(uint8_t onoff)
{
  if (onoff)
    dataport &= ~_BV(LCD_LED_PIN);
  else
    dataport |= _BV(LCD_LED_PIN);
  pcf8574_setoutput(LCD_PCF8574_DEVICEID, dataport);
}

void LCD_home(void)
{
  LCD_command(1 << LCD_HOME);
}

void LCD_putc(char c)
{
  uint8_t pos;

  pos = LCD_waitbusy();   // read busy-flag and address counter
  if (c == '\n')
  {
    LCD_newline(pos);
  }
  else
  {
#if LCD_WRAP_LINES==1
#if LCD_LINES==1
    if ( pos == LCD_START_LINE1 + LCD_DISP_LENGTH ) {
      LCD_write((1 << LCD_DDRAM) + LCD_START_LINE1, 0);
    }
#elif LCD_LINES==2
    if ( pos == LCD_START_LINE1 + LCD_DISP_LENGTH ) {
      LCD_write((1 << LCD_DDRAM) + LCD_START_LINE2, 0);
    } else if ( pos == LCD_START_LINE2 + LCD_DISP_LENGTH ) {
      LCD_write((1 << LCD_DDRAM) + LCD_START_LINE1, 0);
    }
#elif LCD_LINES==4
    if ( pos == LCD_START_LINE1 + LCD_DISP_LENGTH ) {
      LCD_write((1 << LCD_DDRAM) + LCD_START_LINE2, 0);
    } else if ( pos == LCD_START_LINE2 + LCD_DISP_LENGTH ) {
      LCD_write((1 << LCD_DDRAM) + LCD_START_LINE3, 0);
    } else if ( pos == LCD_START_LINE3 + LCD_DISP_LENGTH ) {
      LCD_write((1 << LCD_DDRAM) + LCD_START_LINE4, 0);
    } else if ( pos == LCD_START_LINE4 + LCD_DISP_LENGTH ) {
      LCD_write((1 << LCD_DDRAM) + LCD_START_LINE1, 0);
    }
#endif
    LCD_waitbusy();
#endif
    LCD_write(c, 1);
  }

}

void LCD_puts(const char *s)
{
  register char c;

  while ( (c = *s++) ) {
    LCD_putc(c);
  }

}

