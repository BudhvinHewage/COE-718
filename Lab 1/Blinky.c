/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 * Note(s): __USE_LCD   - enable Output on LCD, uncomment #define in code to use
 *  				for demo (NOT for analysis purposes)
 *----------------------------------------------------------------------------
 * Copyright (c) 2008-2011 Keil - An ARM Company.
 * Name: Anita Tino
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "Blinky.h"
#include "LPC17xx.h"                       
#include "GLCD.h"
#include "LED.h"
#include "Board_ADC.h"
#include "KBD.h" 

#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   0										/* Uncomment to use the LCD */

//ITM Stimulus Port definitions for printf //////////////////
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

struct __FILE { int handle;  };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) {
  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
  return(ch);
}
/////////////////////////////////////////////////////////

char text[10];
char text_l[10];

static volatile uint16_t AD_dbg;

uint16_t ADC_last;                      // Last converted value
/* Import external variables from IRQ.c file                                  */
extern uint8_t  clock_ms;


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  int32_t  res;
  uint32_t AD_sum   = 0U;
  uint32_t AD_cnt   = 0U;
  uint32_t AD_value = 0U;
  uint32_t AD_print = 0U;

  unsigned int led = 0;
  uint32_t current_state = 0;
  uint32_t last_state = 0;


  LED_Init();                                /* LED Initialization            */
  KBD_Init();                                /* Joystick Initialization       */

#ifdef __USE_LCD
  GLCD_Init();                               /* Initialize graphical LCD (if enabled */

  GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(Yellow);
  GLCD_DisplayString(0, 0, __FI, "     COE718 Lab 1   ");
	GLCD_SetTextColor(White);
  GLCD_DisplayString(1, 0, __FI, "       Blinky.c     ");
  GLCD_DisplayString(2, 0, __FI, " Move Joystick for FB ");
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
  GLCD_DisplayString(6, 0, __FI, "Last Direction:         ");
#endif

  //SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/100);       /* Generate interrupt each 10 ms */

  while (1) {                                /* Loop forever                  */

    /* the goal is to establish the current value of a zero value for the state of the 
    joystick and then identify when it changes, and display theose changes on the LCD
    indicating the direction chosen in words, and use the LED to indicate the direction
    as well */

    current_state = get_button();              // Get current state of joystick
    
    if (current_state != last_state) {            // If conversion has finished
      LED_Off(led);                          // Turn off last LED
      if (current_state == 0) {                           // If no button pressed
        GLCD_SetTextColor(White);
        GLCD_DisplayString(6,  9, __FI, (unsigned char *)"NONE  ");
      }
      if (current_state == KBD_SELECT) {                       // If select button pressed
        led = 1;                             // Set last LED to 1
        GLCD_SetTextColor(Blue);
        GLCD_DisplayString(6,  9, __FI, (unsigned char *)"SELECT");
      }
      if (current_state == KBD_UP) {                       // If up button pressed
        led = 2;                             // Set last LED to 2
        GLCD_SetTextColor(Blue);
        GLCD_DisplayString(6,  9, __FI, (unsigned char *)"UP    ");
      }
      if (current_state == KBD_DOWN) {                       // If down button pressed
        led = 3;                             // Set last LED to 3
        GLCD_SetTextColor(Blue);
        GLCD_DisplayString(6,  9, __FI, (unsigned char *)"DOWN  ");
      }
      if (current_state == KBD_LEFT) {                       // If left button pressed
        led = 4;                             // Set last LED to 4
        GLCD_SetTextColor(Blue);
        GLCD_DisplayString(6,  9, __FI, (unsigned char *)"LEFT  ");
      }
      if (current_state == KBD_RIGHT) {                       // If right button pressed
        led = 5;                             // Set last LED to 5
        GLCD_SetTextColor(Blue);
        GLCD_DisplayString(6,  9, __FI, (unsigned char *)"RIGHT ");
      }
      LED_On(led);                                  // Turn on current LED
    }    
			
      last_state = current_state;                          // Update last state
  }
}

