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
int TPS = 268451096;
int ARRIVALRATE = 9;
int SERVICERATE = 10;
int QSIZE = 10;
int msg = NULL;
int currentClient = 1;
int MAXCLIENTS = 	2;
int totalRecieved = 0;
int totalOverflow = 0;
int totalSent = 0;
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
		printf("Total Received: %i\nTotal Sent: %i\nTotal Overflow: %i\n", totalRecieved, totalSent, totalOverflow);
		osDelay(TPS);
	}
}
void client(void *arg){
	osMessageQueueId_t q1 = &arg[0];
	osMessageQueueId_t q2 = &arg[1];
	while(1){
		if(currentClient == MAXCLIENTS){
			osStatus_t stat = osMessageQueuePut(q2, &msg, 0, 0);
			if(stat == osErrorResource){
				totalOverflow++;
			}
			else{
				totalSent++;
			}
			currentClient = 1;
		}
		else{
			osStatus_t stat = osMessageQueuePut(q1, &msg, 0, 0);
			if(stat == osErrorResource){
				totalOverflow++;
			}
			else{
				totalSent++;
			}
			currentClient++;
		}
		osDelay(getTicks(ARRIVALRATE*MAXCLIENTS));//because they are both in this loop it needs to be multiplied
	}
}
void server(void *arg){
	while(1){
		osMessageQueueGet(arg, &msg, NULL, osWaitForever);
		totalRecieved++;
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
