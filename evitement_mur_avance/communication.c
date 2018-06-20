#include <kilombo.h>

#include "evite.h"
#include "communication.h"

extern USERDATA * mydata;

void rxbuffer_push(message_t *msg, distance_measurement_t *dist){
		if(!mydata->wall_detected){
			mydata->wall_detected = 1;
		} 
		mydata->wall_detected_timeout = kilo_ticks + 64;
    received_message_t *rmsg = &RB_back();
    rmsg->msg = *msg;
    rmsg->dist = *dist;
    RB_pushback();
		printf("Head : %d, Tail : %d, Size : %d\n", mydata->RXHead, mydata->RXTail, RB_SIZE);
		if(mydata->RXHead == (mydata->RXTail)+1%RB_SIZE){
			printf("rb full\n");
			update_mur_from_message();
		}
		mydata->new_message = 1;
}

void update_mur_from_message(){
	mydata->x_old = mydata->x_new;
	mydata->x_new = 0;
	int i = 0;
	for(i=0; i<RB_SIZE; i++){
		//mydata->x_new += estimate_distance(&mydata->RXBuffer[i].dist);
		printf("BUFFER%d : %d\n", i, estimate_distance(&mydata->RXBuffer[i].dist));
		distance_measurement_t dist = RB_front().dist;
		mydata->x_new += estimate_distance(&dist);
		//RB_popfront();

	}
	mydata->x_new /= 4;
	printf("x_old : %d, x_new : %d\n",mydata->x_old, mydata->x_new);
	RB_init();
}

message_t *message_tx(){
	if(mydata->broadcast){
		return &(mydata->msg_transmis);
	}
	return 0;
}
