/*
Copyright goes here.
Fredrik Simonsson
*/

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "Timer.h"

void showline(int line,uint8_t *bitmap ){
#define WAITTICK 1
	if (line == 7 ){
		  // Line 7 is on GPIO C bit 1
	      GPIOC->ODR = 2;
	      GPIOB->ODR = 0;
	}else {
	      GPIOC->ODR = 0;
	      GPIOB->ODR = 1<<line;
	}
	      GPIOA->ODR = 0 + bitmap[line];
	      timer_sleep( WAITTICK);
}
int main(int argc, char* argv[]) {
  uint8_t bitmap[8] = {0x66,0x99,0x81,0xc3,0xc3,0x66,0x3c,0x18 };  //Heart


  timer_start();
  uint32_t seconds = 0;
  volatile int x=0;
  // Enable GPIO Peripheral clock
  /* GPIOC Periph clock enable */
  RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN |RCC_AHB1ENR_GPIOBEN |RCC_AHB1ENR_GPIOCEN );

  // Set up direction for GPIOA B C
  // Each gpio bit have 2 control bits
  // 00 input
  // 01 output
  // 10 alternativ function
  // 11 Analog
  GPIOA->MODER = (GPIOA->MODER & 0xFFFF0000) | 0x00005555 ;
  GPIOB->MODER = (GPIOB->MODER & 0xFFFF0000) | 0x00005555 ;
  GPIOC->MODER = (GPIOC->MODER & 0xFFFFFFF3) | 0x00000004 ;


  // Infinite loop
  while (1)
    {
	   showline(x&7,bitmap);
	   x++;
    }
    ++seconds;
}
