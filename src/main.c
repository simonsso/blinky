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
#define C_HEART 0
#define C_SHI 1
#define C_MO  2
#define C_N   3
#define C_SO  4
#define C_STAR 5
  int msg[] = {C_SHI,C_MO,C_N,C_SO,C_N,C_HEART,C_STAR,C_HEART,-1};
  uint8_t bitmap[] = {0x66,0x99,0x81,0xc3,0xc3,0x66,0x3c,0x18,  //Heart
		              0x60,0x02,0x62,0x04,0x08,0x10,0x60,0x00,  //katakana shi
					  0x3c,0x10,0x10,0x7e,0x10,0x10,0x0c,0x00,  //katakana mo
		              0x00,0x00,0x60,0x64,0x04,0x04,0x78,0x00,  //katakana n
		              0x00,0x44,0x44,0x24,0x04,0x18,0x00,0x00,  //katakana so
					  0x00,0x00,0x66,0x18,0x18,0x66,0x00,0x00	//star
                     };

  timer_start();
  (void) argc;
  (void) argv;

  uint32_t seconds = 0;
  volatile int x=0;
  // Enable GPIO Peripheral clock
  /* GPIOC Periph clock enable */
  RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN |RCC_AHB1ENR_GPIOBEN |RCC_AHB1ENR_GPIOCEN );

  // Set up direction for GPIOA B C
  // Each GPIO bit have 2 control bits
  // 00 input
  // 01 output
  // 10 alternative function
  // 11 Analogue
  GPIOA->MODER = (GPIOA->MODER & 0xFFFF0000) | 0x00005555 ;
  GPIOB->MODER = (GPIOB->MODER & 0xFFFF0000) | 0x00005555 ;
  GPIOC->MODER = (GPIOC->MODER & 0xFFFFFFF3) | 0x00000004 ;


   // Infinite loop
   volatile int i=0;
   int cindex=msg[0];
   while (1){
      showline(x&7,&bitmap[8*cindex]);
      x++;
      if( x == 2000 ){
    	  i++;
    	  cindex=msg[i];

    	  x=0;
    	  if(cindex == -1 ){
    		  i=0;
    		  cindex=msg[0];
    	  }
      }
    }
    ++seconds;
}
