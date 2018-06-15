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


void update_mur_from_message(){
	mydata->mur_dist=mydata->message_dist;
	mydata->mur_update=kilo_ticks;
	mydata->new_message=0;
}

void update_from_message(){
	int ID=mydata->message.data[0];

	if(isMur(ID)) return update_mur_from_message();

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



void message_rx (message_t *message, distance_measurement_t *distance){
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
