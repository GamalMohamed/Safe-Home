#include "lcd.h"

void setup() {
  LCD_init(); /* initialize LCD */
  LCD_displayStringRowColumn(0,2,"WELCOME!");
  LCD_displayStringRowColumn(1,3,"SAFE HOME");
  _delay_ms(300); /* wait four seconds */

  LCD_clearScreen(); /* clear the LCD display */
  LCD_displayString("Activate System?");
  LCD_displayStringRowColumn(1,5,"Y/N");

}

void loop() {
  
}
