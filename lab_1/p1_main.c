/****************************************************************************
 * This code is copyright (c) 2017 by the University of Waterloo and may    *
 * not be redistributed without explicit written permission.                *
 ****************************************************************************/
 
#include <LPC17xx.h>
#include "stdio.h"
#include "uart.h"
#define part_4
int main( void ) 
{
	#ifdef part_1
	LPC_GPIO2->FIODIR = 0x1f<<2; // Configure pins 2 to 6 on Port 0 as Output
	LPC_GPIO1->FIODIR=0xf<<28;//configure pin 28-31 as output led
	while(1){
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){
			LPC_GPIO2->FIOSET = 1 << 6; // Output HIGH
		}
		else{
			LPC_GPIO2->FIOCLR = 1 << 6; // Output LOW
		}
	}
	
	
	#endif
	
	#ifdef part_2
	printf("2");
	#endif
	
	#ifdef part_3
	printf("3");
	#endif
	
	#ifdef part_4
	LPC_SC->PCONP |= 1 << 12;
	#endif
}