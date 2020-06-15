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
	LPC_GPIO1->FIODIR=0xf<<28;//configure pin 28-31 as output led and set to low
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
	while (1) {
		if((LPC_GPIO1->FIOPIN & (1 << 20)) == 0) //  PRESSED
		{
			if (((LPC_GPIO1->FIOPIN & (1 << 23)) == 0)) // NORTH
				printf("North, Pressed\n");
			else if (((LPC_GPIO1->FIOPIN & (1 << 24)) == 0)) // EAST
				printf("East, Pressed\n");
			else if (((LPC_GPIO1->FIOPIN & (1 << 25)) == 0)) // SOUTH
				printf("South, Pressed\n");
			else if (((LPC_GPIO1->FIOPIN & (1 << 26)) == 0)) // WEST
				printf("West, Pressed\n"); 
			else
			printf("Pressed\n"); // PRESSED, NO JOYSTICK MOVEMENT 
		}
		else if ((LPC_GPIO1->FIOPIN & (1 << 20)) != 0)
		{
			if (((LPC_GPIO1->FIOPIN & (1 << 23)) == 0)) // NORTH
				printf("North, Not Pressed\n");
			else if (((LPC_GPIO1->FIOPIN & (1 << 24)) == 0)) // EAST
				printf("East, Not Pressed\n");
			else if (((LPC_GPIO1->FIOPIN & (1 << 25)) == 0)) // SOUTH
				printf("South, Not Pressed\n");
			else if (((LPC_GPIO1->FIOPIN & (1 << 26)) == 0)) // WEST
				printf("West, Not Pressed\n"); 
			else
			printf(" Not Pressed\n"); // NOT PRESSED, NO JOYSTICK MOVEMENT
		}
	}
	#endif
	
	#ifdef part_3
	// CLEAR LED OUPUTS 
	LPC_GPIO1->FIOCLR = 1 << 28;
	LPC_GPIO1->FIOCLR = 1 << 29;
	LPC_GPIO1->FIOCLR = 1u << 31;
	LPC_GPIO2->FIOCLR = 1 << 2;
	LPC_GPIO2->FIOCLR = 1 << 3;
	LPC_GPIO2->FIOCLR = 1 << 4;
	LPC_GPIO2->FIOCLR = 1 << 5;
	LPC_GPIO2->FIOCLR = 1 << 6;
	while(1) {
		int inputVal; // INPUT 
		char input[3];
		printf("enter value\n");
		scanf("%s", input);
			// SET LED OUPUTS
	LPC_GPIO1->FIODIR |= 1 << 28;
	LPC_GPIO1->FIODIR |= 1 << 29;
	LPC_GPIO1->FIODIR |= 1u << 31;
	LPC_GPIO2->FIODIR |= 1 << 2;
	LPC_GPIO2->FIODIR |= 1 << 3;
	LPC_GPIO2->FIODIR |= 1 << 4;
	LPC_GPIO2->FIODIR |= 1 << 5;
	LPC_GPIO2->FIODIR |= 1 << 6;
	// CLEAR LED OUPUTS 
	LPC_GPIO1->FIOCLR = 1 << 28;
	LPC_GPIO1->FIOCLR = 1 << 29;
	LPC_GPIO1->FIOCLR = 1u << 31;
	LPC_GPIO2->FIOCLR = 1 << 2;
	LPC_GPIO2->FIOCLR = 1 << 3;
	LPC_GPIO2->FIOCLR = 1 << 4;
	LPC_GPIO2->FIOCLR = 1 << 5;
	LPC_GPIO2->FIOCLR = 1 << 6;
	int output[] = {0,0,0,0,0,0,0,0};
		int count = 0; // OUTPUT TO LEDS AND COUNTER
		inputVal = atoi(input);
		while (inputVal > 0) {
			output[count] = inputVal % 2;
			inputVal = inputVal/2;
			count++;
		}
		LPC_GPIO1->FIOSET |= (output[2] << 31);
		for (int x = 0; x < 2; x++) { // LED FUNCTION AT 28, 29
			LPC_GPIO1->FIOSET |= (output[x] << (28+x));
		}
		LPC_GPIO1->FIOSET |= (output[2] << 31);
		for (int x = 2; x < 7; x++) {	
			LPC_GPIO2->FIOSET |= (output[x+1] << x); // LED FUNCTION AT , 2,3,4,5,6
		}
	}
	#endif
	
	#ifdef part_4
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
			val = LPC_ADC->ADGDR & (0xfff << 4);
			val /= 1241*16;
			printf("%f\n", val);
		}
	}
	#endif
}
