/****************************************************************************
 * This code is copyright (c) 2017 by the University of Waterloo and may    *
 * not be redistributed without explicit written permission.                *
 ****************************************************************************/
 
#include <LPC17xx.h>
#include "stdio.h"
#include "uart.h"
#define part_1
void delay(void);
int main( void ) 
{
	#ifdef part_1
	LPC_GPIO2->FIODIR = 0xF; // Configure pins 0 to 3 on Port 0 as Output
	
	while(1)
	{
		LPC_GPIO2->FIOSET = 0xF; // Output HIGH
		delay();
		LPC_GPIO2->FIOCLR = 0xF; // Output LOW
		delay();
	}
	return 0; // normally this wont execute
	#endif
	
	#ifdef part_2
	printf("2");
	#endif
	
	#ifdef part_3
	printf("3");
	#endif
	
	#ifdef part_4
	LPC_SC->PCONP |= 1 << 5;
	#endif
}
void delay(void) //Hardcoded delay function
{
	int count,i=0;
	for(count=0; count < 6000000; count++) // You can edit this as per your needs
	{
		i++; // something needs to be here else compiler will remove the for loop!
	}
}