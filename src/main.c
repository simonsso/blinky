/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "Timer.h"
//#include "BlinkLed.h"

// ----------------------------------------------------------------------------
//
// Standalone STM32F4 led blink sample (trace via DEBUG).
//
// In debug configurations, demonstrate how to print a greeting message
// on the trace device. In release configurations the message is
// simply discarded.
//
// Then demonstrates how to blink a led with 1 Hz, using a
// continuous loop and SysTick delays.
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//


// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

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
int main(int argc, char* argv[])
{
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

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
