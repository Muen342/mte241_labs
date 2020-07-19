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
int TPS = 268451096;//ticks per second
int ARRIVALRATE = 9;
int SERVICERATE = 10;
int QSIZE = 10;
int msg = NULL;
int currentClient = 1;
int MAXCLIENTS = 	2;
int totalRecieved1 = 0;
int totalOverflow1 = 0;
int totalSent1 = 0;
int totalRecieved2 = 0;
int totalOverflow2 = 0;
int totalSent2 = 0;
uint32_t getTicks(rate){
	uint32_t nevent = next_event();
	nevent = nevent*TPS/rate;
	nevent = (nevent>>16);
	return nevent;
}

osMessageQueueId_t createNewQ(){
	osMessageQueueId_t q_id = osMessageQueueNew(QSIZE, sizeof(int), NULL);
	return q_id;
}

void monitor(void *arg){
	while(1){
		printf("Queue 1:\nTotal Received: %i\nTotal Sent: %i\nTotal Overflow: %i\n", totalRecieved1, totalSent1, totalOverflow1);// displays monitored values for q1 and q2
		printf("Queue 2:\nTotal Received: %i\nTotal Sent: %i\nTotal Overflow: %i\n", totalRecieved2, totalSent2, totalOverflow2);
		osDelay(TPS);
	}
}
void client(void *arg){
	osMessageQueueId_t q1 = &arg[0];  // assigns q1 and q2 arg array positions
	osMessageQueueId_t q2 = &arg[1];
	while(1){
		if(currentClient == MAXCLIENTS){
			osStatus_t stat = osMessageQueuePut(q2, &msg, 0, 0); // status of q2 as message is sent to q2
			if(stat == osErrorResource){
				totalOverflow2++;
			}
			else{
				totalSent2++;
			}
			currentClient = 1;
		}
		else{
			osStatus_t stat = osMessageQueuePut(q1, &msg, 0, 0); // status of q1 as message is sent to q1
			if(stat == osErrorResource){
				totalOverflow1++;// inc overflow val
			}
			else{
				totalSent1++;
			}
			currentClient++;
		}
		osDelay(getTicks(ARRIVALRATE*MAXCLIENTS));//because they are both in this loop it needs to be multiplied
	}
}
void server(void *arg){
	while(1){
		osMessageQueueGet(&arg, &msg, NULL, osWaitForever);
		//find way to find out which q it is servicing
		totalRecieved1++;
		osDelay(getTicks(SERVICERATE));
	}
}
int main( void ) 
{
	osMessageQueueId_t q1 = createNewQ();
	osMessageQueueId_t q2 = createNewQ();
	osMessageQueueId_t clients[2] = {q1, q2};
	osKernelInitialize();
	osThreadNew(client, clients, NULL);
	osThreadNew(server, q1, NULL);
	osThreadNew(server, q2, NULL);
	osThreadNew(monitor, NULL, NULL);
	osKernelStart();
}
