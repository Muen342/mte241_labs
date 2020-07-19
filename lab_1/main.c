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
		//do the displaying here
		osDelay(TPS);
	}
}
void client(void *arg){
	osMessageQueueId_t q1 = createNewQ();
	osMessageQueueId_t q2 = createNewQ();
	while(1){
		if(currentClient == MAXCLIENTS){
			osMessageQueuePut(q2, &msg, 0, 0);
			currentClient = 1;
		}
		else{
			osMessageQueuePut(q1, &msg, 0, 0);
			currentClient++;
		}
		osDelay(getTicks(ARRIVALRATE*MAXCLIENTS));//because they are both in this loop it needs to be multiplied
	}
}
void server(void *arg){
	while(1){
		//find a way to pass in the qid into this thread and then deal with it with
		//osMessageQueueGet(q_id, &msg, NULL, osWaitForever);
		osDelay(getTicks(SERVICERATE));
	}
}
int main( void ) 
{
	osKernelInitialize();
	osKernelStart();
}
