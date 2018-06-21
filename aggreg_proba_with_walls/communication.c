#include <kilombo.h>

#include "aggregation.h"
#include "communication.h"

extern USERDATA * mydata;

void update_voisins(){
	if (!mydata->nb_voisins){
		return;
	}
	int i;
	for(i=mydata->nb_voisins-1;i>=0;i--){
		if (kilo_ticks-mydata->voisins_liste[i].timestamp > SECONDE){
			mydata->voisins_liste[i]=mydata->voisins_liste[mydata->nb_voisins-1];
			mydata->voisins_liste[mydata->nb_voisins-1].id=-1;
			mydata->nb_voisins--;
		}
	}
}


void compute_wall_mean(){
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

void update_from_message(){
	int ID=mydata->message.data[0];

	// if(isMur(ID)) return update_mur_from_message();

	uint8_t found_id=0;
	uint32_t distance=mydata->message_dist;
	int i=0;
	while (i< mydata->nb_voisins && !found_id ){
		if(mydata->voisins_liste[i].id ==ID){
			found_id=1;
		}else{
			i++;
		}
	}

	if (!found_id){
		if (mydata->nb_voisins<MAXVOISIN){
			mydata->nb_voisins++;
		}else{
			i--;
		}
	}

		mydata->voisins_liste[i].id=ID;
		mydata->voisins_liste[i].timestamp=kilo_ticks;
		mydata->voisins_liste[i].dist=distance;
		mydata->voisins_liste[i].nb_voisins=mydata->message.data[6];

	mydata->new_message=0;
}

void rxbuffer_push(message_t *msg, distance_measurement_t *dist){

		mydata->wall_detected_timeout = kilo_ticks + 64;
    received_message_t *rmsg = &RB_back();
    rmsg->msg = *msg;
    rmsg->dist = *dist;
    RB_pushback();
		// printf("Head : %d, Tail : %d, Size : %d\n", mydata->RXHead, mydata->RXTail, RB_SIZE);
		if(mydata->RXHead == (mydata->RXTail)+1%RB_SIZE){ //RB_full()
			// printf("rb full\n");
			compute_wall_mean();
		}
		mydata->wall_detected = 1;
}

void message_rx (message_t *message, distance_measurement_t *distance){
	if(isMur(message->data[0])) return rxbuffer_push(message,distance);

    mydata->new_message = 1;
		mydata->message=*message;
		mydata->message_dist=estimate_distance(distance);
	//	mydata->toAggregate.dist = estimate_distance(distance);
		//if (kilo_uid==8) printf("DISTANCE.toAggregate : %d\n",mydata->toAggregate.dist);
}


//WORK IN PROGRESS
void setup_message(){
	mydata->broadcast=0;//ne pas transmettre quand on change le message

	mydata->msg_transmis.type = NORMAL;
	mydata->msg_transmis.data[0] = kilo_uid; //on sait que le kilo_uid < 256 car on a pas autant de kilobot
	mydata->msg_transmis.data[6]=mydata->nb_voisins;
	mydata->msg_transmis.crc = message_crc(&mydata->msg_transmis);

	mydata->broadcast=1;
	return;
}



message_t *message_tx(){
	if(mydata->broadcast){
		return &(mydata->msg_transmis);
	}
	return 0;
}
