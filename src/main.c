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
   volatile int x=0;
   // Enable GPIO Peripheral clock
   /* GPIOC Periph clock enable */
   RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN |RCC_AHB1ENR_GPIOBEN );

   // Set up direction for GPIOA B 
   // Each gpio bit have 2 control bits
   // 00 input
   // 01 output
   // 10 alternativ function
   // 11 Analog
   GPIOA->MODER = (GPIOA->MODER & 0xFFFF0000) | 0x00005555 ;
   GPIOB->MODER = (GPIOB->MODER & 0xFFFF0000) | 0x00005555 ;


   // Infinite loop
   while (1) {
#define WAITTICK 1000
      GPIOA->ODR = 0;
      timer_sleep( WAITTICK);
      GPIOA->ODR = 0xffff;
      timer_sleep( WAITTICK);
      x++;
   }
}
