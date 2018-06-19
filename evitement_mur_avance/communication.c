#include <kilombo.h>

#include "evite.h"
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

void rxbuffer_push(message_t *msg, distance_measurement_t *dist){
    received_message_t *rmsg = &RB_back();
    rmsg->msg = *msg;
    rmsg->dist = *dist;
    RB_pushback();
		mydata->new_message = 1;
		mydata->cpt_message++;
		printf("cpt_message : %d\n",mydata->cpt_message );
}

void update_mur_from_message(){
	int x_old = 0;
	int x_new = 0;
	if(RB_full()){
		if(mydata->cpt_message == RB_SIZE){
			mydata->cpt_message = 0;
			printf("je rentre ici\n");
			int i = 0;
			for(i=0; i<RB_SIZE/2; i++){
				x_old += estimate_distance(&mydata->RXBuffer[i].dist);
			}
			x_old /= 4;
			for(i=RB_SIZE/2;i<RB_SIZE;i++){
				x_new += estimate_distance(&mydata->RXBuffer[i].dist);
			}
			x_new /= 4;

			printf("x_old : %d, x_new : %d\n",x_old, x_new);
			mydata->x_old = x_old;
			mydata->x_new = x_new;
		}
	}
}

void update_from_message(){

	uint8_t *data = RB_front().msg.data;
	int ID=data[0];
	if(isMur(ID)){
		if(!mydata->wall_detected){
			mydata->wall_detected = 1;
		}
		mydata->wall_detected_timeout = kilo_ticks + 64;
		update_mur_from_message();
	}

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

	//mydata->new_message=0;
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
