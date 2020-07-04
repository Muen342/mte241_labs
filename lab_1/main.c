/****************************************************************************
 * This code is copyright (c) 2017 by the University of Waterloo and may    *
 * not be redistributed without explicit written permission.                *
 ****************************************************************************/
 
#include <LPC17xx.h>
#include "stdio.h"
#include "uart.h"
#include "stdlib.h"
#include "stdbool.h"
#include "ctype.h"
#include <cmsis_os2.h>
#define part_1

int prevbutton = 1;
int prevstate = 0;

void button(void *arg){
	LPC_GPIO2->FIODIR = 0x1f<<2; // Configure pins 2 to 6 on Port 0 as Output
	LPC_GPIO1->FIODIR=0xf<<28;//configure pin 28-31 as output led and set to low
	while(1){
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){
			prevbutton = 0;
		}
		else{
			if(prevbutton == 0 && prevstate == 0){
				LPC_GPIO2->FIOSET = 1 << 6; // Output HIGH
				prevstate = 1;
			}
			else if(prevbutton == 0 && prevstate == 1){
				LPC_GPIO2->FIOCLR = 1 << 6; // Output LOW
				prevstate = 0;
			}
			prevbutton = 1;
		}
		osThreadYield();
	}
}

void ADC(void *arg){
	LPC_SC->PCONP |= 1 << 12;
	LPC_PINCON->PINSEL1 &= ~(0x03 <<18);//clear this value
	LPC_PINCON->PINSEL1 |= (0x01<<18);//set it as adc ad0.2
	LPC_ADC->ADCR |= 4 << 8;//divide by 4+1 so 5 mhz
	LPC_ADC->ADCR |= 1 <<21;//enable circuit
	LPC_ADC->ADCR |= 1 << 2;//sets channel of input
	LPC_ADC -> ADCR &= ~1;//bit 1 was on for some reason
	float val = 0;
	while(1){
		LPC_ADC->ADCR |= 1 <<24;//start conversion
		if((LPC_ADC->ADGDR & (1<<31)) == 0){
		}
		else{
			val = LPC_ADC->ADGDR >> 4 & 0xfff;
			val /= 1241;
			printf("%f\n", val);
		}
		osThreadYield();
	}
}

void joystick(void *arg){
	while (1) {
		if((LPC_GPIO1->FIOPIN & (1 << 20)) == 0) //  PRESSED
		{
			if (((LPC_GPIO1->FIOPIN & (1 << 23)) == 0)) 
			{	// NORTH, SHOW 001
				LPC_GPIO2->FIOSET = 1 << 2;
				LPC_GPIO1->FIOSET = 1 << 28;
				LPC_GPIO1->FIOCLR = 1 << 29;
				LPC_GPIO1->FIOCLR = 1u << 31;
			}
			else if (((LPC_GPIO1->FIOPIN & (1 << 24)) == 0)) 
			{ 	// EAST, SHOW 010
				LPC_GPIO2->FIOSET = 1 << 2;
				LPC_GPIO1->FIOCLR = 1 << 28;
				LPC_GPIO1->FIOSET = 1 << 29;
				LPC_GPIO1->FIOCLR = 1u << 31;
			}
			else if (((LPC_GPIO1->FIOPIN & (1 << 25)) == 0)) 
			{	// SOUTH, SHOW 011
				LPC_GPIO2->FIOSET = 1 << 2;
				LPC_GPIO1->FIOSET = 1 << 28;
				LPC_GPIO1->FIOSET = 1 << 29;
				LPC_GPIO1->FIOCLR = 1u << 31;	
			}
			else if (((LPC_GPIO1->FIOPIN & (1 << 26)) == 0)) 
			{	// WEST, SHOW 100
				LPC_GPIO2->FIOSET = 1 << 2;
				LPC_GPIO1->FIOCLR = 1 << 28;
				LPC_GPIO1->FIOCLR = 1 << 29;
				LPC_GPIO1->FIOSET = 1u << 31;
			}
			else
			{
				LPC_GPIO2->FIOCLR = 1 << 2;
				LPC_GPIO1->FIOCLR = 1 << 28;
				LPC_GPIO1->FIOCLR = 1 << 29;
				LPC_GPIO1->FIOCLR = 1u << 31;
			}
		}
		else if ((LPC_GPIO1->FIOPIN & (1 << 20)) != 0)
		{
			if (((LPC_GPIO1->FIOPIN & (1 << 23)) == 0)) 
			{	// NORTH, SHOW 001
				LPC_GPIO2->FIOCLR = 1 << 2;
				LPC_GPIO1->FIOSET = 1 << 28;
				LPC_GPIO1->FIOCLR = 1 << 29;
				LPC_GPIO1->FIOCLR = 1u << 31;
			}
			else if (((LPC_GPIO1->FIOPIN & (1 << 24)) == 0)) 
			{	// EAST, SHOW 010
				LPC_GPIO2->FIOCLR = 1 << 2;
				LPC_GPIO1->FIOCLR = 1 << 28;
				LPC_GPIO1->FIOSET = 1 << 29;
				LPC_GPIO1->FIOCLR = 1u << 31;
				
			}
			else if (((LPC_GPIO1->FIOPIN & (1 << 25)) == 0)) 
			{	// SOUTH, SHOW 011
				LPC_GPIO2->FIOCLR = 1 << 2;
				LPC_GPIO1->FIOSET = 1 << 29;
				LPC_GPIO1->FIOSET = 1 << 28;
				LPC_GPIO1->FIOCLR = 1u << 31;
			}
			else if (((LPC_GPIO1->FIOPIN & (1 << 26)) == 0)) 
			{	// WEST, SHOW 100
				LPC_GPIO2->FIOCLR = 1 << 2;
				LPC_GPIO1->FIOCLR = 1 << 28;
				LPC_GPIO1->FIOCLR = 1 << 29;
				LPC_GPIO1->FIOSET = 1u << 31;
			}
			else
			{	
				LPC_GPIO2->FIOCLR = 1 << 2;
				LPC_GPIO1->FIOCLR = 1 << 28;
				LPC_GPIO1->FIOCLR = 1 << 29;
				LPC_GPIO1->FIOCLR = 1u << 31;
			}
		}
		osThreadYield();
	}
}

int main( void ) 
{
	LPC_GPIO2->FIODIR = 0x1f<<2; // Configure pins 2 to 6 on Port 0 as Output
	LPC_GPIO1->FIODIR=0xf<<28;//configure pin 28-31 as output led and set to low
	osKernelInitialize();
	osThreadNew(button, NULL, NULL);
	osThreadNew(ADC, NULL, NULL);
	//osThreadNew(joystick, NULL, NULL);
	osKernelStart();
}
