/*
Copyright goes here.
Fredrik Simonsson
*/

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "Timer.h"
#define WAITTICK 1
void showline(int line,uint8_t *bitmap ){
	if (line == 7 ){
		  // Line 7 is on GPIO C bit 1
	      GPIOC->ODR = 2;
	      GPIOB->ODR = 0;
	}else {
	      GPIOC->ODR = 0;
	      GPIOB->ODR = 1<<line;
	}
	      GPIOA->ODR = 0 + *bitmap;
	      timer_sleep( WAITTICK);
}

void showline_hscroll(int line,int shift,uint8_t *bitmap0, uint8_t *bitmap1 ){
	if (line == 7 ){
		  // Line 7 is on GPIO C bit 1
	      GPIOC->ODR = 2;
	      GPIOB->ODR = 0;
	}else {
	      GPIOC->ODR = 0;
	      GPIOB->ODR = 1<<line;
	}
	uint32_t bitmap = (bitmap0[line]*256)+bitmap1[line];
	bitmap = bitmap >> ( 8 - (7&shift));
	GPIOA->ODR = bitmap;
	timer_sleep( WAITTICK);
}
int main(int argc, char* argv[]) {
#define C_HEART 0
#define C_SHI 1
#define C_MO  2
#define C_N   3
#define C_SO  4
#define C_STAR 5
#define C_CHESS 6
#define C_DOT 7
  int msg[] = {C_CHESS,C_DOT,C_CHESS,C_DOT,C_HEART,C_DOT,C_HEART,C_SHI,C_MO,C_N,C_SO,C_N,C_HEART,C_STAR,C_HEART,C_DOT,-1};
  uint8_t bitmap[] = {0x66,0x99,0x81,0xc3,0xc3,0x66,0x3c,0x18,  //Heart
		              0x60,0x02,0x62,0x04,0x08,0x10,0x60,0x00,  //katakana shi
					  0x3c,0x10,0x10,0x7e,0x10,0x10,0x0c,0x00,  //katakana mo
		              0x00,0x00,0x60,0x64,0x04,0x04,0x78,0x00,  //katakana n
		              0x00,0x44,0x44,0x24,0x04,0x18,0x00,0x00,  //katakana so
					  0x00,0x00,0x66,0x18,0x18,0x66,0x00,0x00,	//star
					  0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,  //chess
					  0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00   //dot
                     };

  timer_start();
  (void) argc;
  (void) argv;

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
  GPIOC->MODER = (GPIOC->MODER & 0xF3FFFFF3) | 0x00000004 ;

  volatile int i=0;
   // Infinite loop
   while(1){

      for(int vbl=0;vbl<8;vbl++){
          uint32_t cindex=((i+vbl)&~7)/8;
          if(msg[cindex] == -1 ){
                  cindex=0;
          }
          uint32_t cindex0=((i)&~7)/8;
          uint32_t cindex1=((i+8)&~7)/8;

    	  if(msg[cindex0] == -1 ){
    		  cindex0=0;
    	  }
    	  if(msg[cindex1] == -1 ){
    		  cindex1=0;
    	  }

    	  if (GPIOC->IDR & 0x2000){
              int line = ((i+vbl)&7);
              showline(vbl&7,&bitmap[(8*msg[cindex])+line]);
    	  }else{
    	  	  int line = ((i)&7);
             showline_hscroll(vbl,line,&bitmap[(8*msg[cindex0])],&bitmap[(8*msg[cindex1])] );
    	  }
	  }
      x++;
      if( x== 20){
          i++;
    	  x=0;
    	  if(msg[i/8] == -1 ){
    		  i=0;
    	  }
      }
    }
}
