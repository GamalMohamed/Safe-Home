/*
 * lcd.h
 *
 * Created: 5/14/2016 4:41:04 PM
 * Author: Gamal Mohamed
 */ 


#ifndef LCD_H
#define LCD_H

#include "pcf8574.h"

#define LCD_PCF8574_INIT 1 
#define LCD_PCF8574_DEVICEID 0 //device id, addr = pcf8574 base addr + LCD_PCF8574_DEVICEID


/* Display size data */
#define LCD_LINES           2    
#define LCD_DISP_LENGTH    16     
#define LCD_LINE_LENGTH  0x40     
#define LCD_START_LINE1  0x00     
#define LCD_START_LINE2  0x40     
#define LCD_START_LINE3  0x14     
#define LCD_START_LINE4  0x54     
#define LCD_WRAP_LINES      1     


/* LCD Pins */
#define LCD_DATA0_PIN 4
#define LCD_DATA1_PIN 5
#define LCD_DATA2_PIN 6
#define LCD_DATA3_PIN 7
#define LCD_RS_PIN    0
#define LCD_RW_PIN    1
#define LCD_E_PIN     2
#define LCD_LED_PIN   3


/* LCD Commands */
#define LCD_CLR               0      
#define LCD_HOME              1      
#define LCD_ENTRY_MODE        2      
#define LCD_ENTRY_INC         1      /*   DB1: 1=increment, 0=decrement     */
#define LCD_ENTRY_SHIFT       0      /*   DB2: 1=display shift on           */
#define LCD_ON                3      
#define LCD_ON_DISPLAY        2      
#define LCD_ON_CURSOR         1      
#define LCD_ON_BLINK          0      
#define LCD_MOVE              4      
#define LCD_MOVE_DISP         3      /*   DB3: move display (0-> cursor) ?  */
#define LCD_MOVE_RIGHT        2      /*   DB2: move right (0-> left) ?      */
#define LCD_FUNCTION          5      /*   DB5: function set                 */
#define LCD_FUNCTION_8BIT     4      /*   DB4: set 8BIT mode (0->4BIT mode) */
#define LCD_FUNCTION_2LINES   3      /*   DB3: two lines (0->one line)      */
#define LCD_FUNCTION_10DOTS   2      /*   DB2: 5x10 font (0->5x7 font)      */
#define LCD_CGRAM             6      
#define LCD_DDRAM             7      
#define LCD_BUSY              7      


/* Display properties */
#define LCD_DISP_OFF             0x08   
#define LCD_DISP_ON              0x0C   
#define LCD_DISP_ON_BLINK        0x0D   
#define LCD_DISP_ON_CURSOR       0x0E   
#define LCD_DISP_ON_CURSOR_BLINK 0x0F   


/* move cursor/shift display */
#define LCD_MOVE_CURSOR_LEFT     0x10   
#define LCD_MOVE_CURSOR_RIGHT    0x14   
#define LCD_MOVE_DISP_LEFT       0x18   
#define LCD_MOVE_DISP_RIGHT      0x1C   


/* function set: set interface data length and number of display lines */
#define LCD_FUNCTION_4BIT_1LINE  0x20   
#define LCD_FUNCTION_4BIT_2LINES 0x28   

#define LCD_MODE_DEFAULT     ((1<<LCD_ENTRY_MODE) | (1<<LCD_ENTRY_INC) )



/* Initialize display and select type of cursor */
void LCD_init(uint8_t dispAttr=LCD_DISP_ON);

/* Clear display and set cursor to home position */ 
void LCD_clearScreen(void);

/* Set cursor to home position */
void LCD_home(void);

/* Set cursor to specified position */
void LCD_gotoxy(uint8_t x, uint8_t y);

/* Set illumination pin */
void LCD_led(uint8_t onoff);

/* Display character at current cursor position */
void LCD_putc(char c);

/* Display string without auto linefeed */
void LCD_puts(const char *s);

/* Send LCD controller instruction command */
void LCD_command(uint8_t cmd);

/* Send data byte to LCD controller, Similar to lcd_putc(), but without interpreting LF */
void LCD_data(uint8_t data);

#endif //LCD_H
