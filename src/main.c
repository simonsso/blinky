/*
Copyright goes here.
Fredrik Simonsson
*/

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "Timer.h"

int main(int argc, char* argv[]) {
   timer_start();
   (void) argc;
   (void) argv;
   // Enable GPIO Peripheral clock
   /* GPIOC Periph clock enable */
   RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN |RCC_AHB1ENR_GPIOBEN );

   // Set up direction for GPIOA B 
   // Each GPIO bit have 2 control bits
   // 00 input
   // 01 output
   // 10 alternative function
   // 11 Analogue
   GPIOA->MODER = (GPIOA->MODER & 0xFFFF0000) | 0x00005555 ;
   GPIOB->MODER = (GPIOB->MODER & 0xFFFF0000) | 0x00005555 ;

#define WAITTICK 1000

   // Infinite loop
   while (1) {
#if 0
	   // Implementation 1 Write entire port
      GPIOA->ODR = 0;
      timer_sleep( WAITTICK);
      GPIOA->ODR = 0xffff;
      timer_sleep( WAITTICK);
#else
      //Implementation 2 Turn on single bit (PA bit 5 is a LED on nucleo boards)
      GPIOA->BSRR=0x00000020;
      timer_sleep( WAITTICK);
      //turn led off
      GPIOA->BSRR=0x00200000;
      timer_sleep( WAITTICK);
#endif
   }
}
