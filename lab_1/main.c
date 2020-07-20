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
#include <rtx_os.h>
#include "random.h"
#include "lfsr113.h"
int ARRIVALRATE = 9;
int SERVICERATE = 10;
int QSIZE = 10;
int msg = 0;
int currentClient = 1;
int MAXCLIENTS = 	2;
int totalRecieved1 = 0;
int totalOverflow1 = 0;
int totalSent1 = 0;
int totalRecieved2 = 0;
int totalOverflow2 = 0;
int totalSent2 = 0;
int currentTime = 0;
int firstDisplay = 0;
osMessageQueueId_t q1;
osMessageQueueId_t q2;

uint32_t getTicks(rate){
	uint32_t nevent = next_event();
	nevent = nevent*osKernelGetTickFreq()/rate;
	nevent = (nevent>>16);
	return nevent;
}

void client(void *arg){
	while(1){
		if(currentClient == MAXCLIENTS){
			osStatus_t stat = osMessageQueuePut(q2, &msg, 0, 0); // status of q2 as message is sent to q2
			if(stat == osErrorResource){
				totalOverflow2++;
			}
			else if(stat == osOK){
				totalSent2++;
			}
			currentClient = 1;
		}
		else{
			osStatus_t stat = osMessageQueuePut(q1, &msg, 0, 0); // status of q1 as message is sent to q1
			if(stat == osErrorResource){
				totalOverflow1++;// inc overflow val
			}
			else if(stat == osOK){
				totalSent1++;
			}
			currentClient++;
		}
		osDelay(getTicks(ARRIVALRATE*MAXCLIENTS));//because they are both in this loop it needs to be multiplied also yields to the other threads
	}
}
void monitor(void *arg){
	while(1){
		if(firstDisplay == 0){
			firstDisplay++;
			osDelay(osKernelGetTickFreq());
		}
		else{
			osDelay(0.85*osKernelGetTickFreq());
		}
		currentTime++;
		printf("Current Time: %is\n", currentTime);
		printf("Queue 1:  Total Received: %i  Total Sent: %i  Total Overflow: %i\n", totalRecieved1, totalSent1, totalOverflow1);// displays monitored values for q1 and q2
		printf("Queue 2:  Total Received: %i  Total Sent: %i  Total Overflow: %i\n", totalRecieved2, totalSent2, totalOverflow2);
	}
}
void server(void *arg){
	while(1){
		osMessageQueueGet(arg, &msg, NULL, osWaitForever);
		//find way to find out which q it is servicing
		if(arg == q1){
			totalRecieved1++;
		}
		else if(arg == q2){
			totalRecieved2++;
		}
		osDelay(getTicks(SERVICERATE));
	}
}
int main( void ) 
{
	osKernelInitialize();
	q1 = osMessageQueueNew(QSIZE, sizeof(int), NULL);//initialize them
	q2 = osMessageQueueNew(QSIZE, sizeof(int), NULL);
	osThreadNew(client, NULL, NULL);
	osThreadNew(server, q1, NULL);
	osThreadNew(server, q2, NULL);
	osThreadNew(monitor, NULL, NULL);
	osKernelStart();
}