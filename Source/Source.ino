#include"lcd.h"
#include"keypad.h"

unsigned char key;
void setup() {
  LCD_init();
  LCD_displayStringRowColumn(0,1,"Welcome to Safe");
  LCD_displayStringRowColumn(1,3,"Home system!");
  _delay_ms(2000);
  
  LCD_clearScreen();
  LCD_displayStringRowColumn(0,0,"Enter password: ");
  LCD_goToRowColumn(1,0);
}

void loop() {
  /* if any switch pressed for more than 500 ms it counts more than one press */  
    key = KeyPad_getPressedKey(); /* get the pressed key number */
    if((key <= 9) && (key >= 0))
    {
      LCD_intgerToString(key); /* display the pressed keypad switch */
      _delay_ms(500); /* Press time */
    }
    else
    {
      LCD_displayCharacter(key); /* display the pressed keypad switch */ 
      _delay_ms(500); /* Press time */
    }
}
