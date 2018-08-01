#include <kilombo.h>

#include "run_tumble.h"
#include "communication.h"

extern USERDATA * mydata;

int fitness() //minimiser nombre de rencontre unique
{
	// return fitness2();
	return mydata->nb_genome;
}

int fitness2()
{
	return mydata->cpt_msg;
}

void update_from_message(){
	// if (ID==IDFOOD) return;
	if (mydata->message.data[8]==255) return; //255 correspond à un kilobot dead (voir fonction fitness dans mEDEA.c)
	mydata->cpt_msg++;
	uint8_t found_id=0;
	uint8_t i=0;
	uint8_t ID=mydata->message.data[0];
		while (i< mydata->nb_genome && !found_id ){
			if(mydata->genome_list[i].id ==ID){
				found_id=1;

			}else{
				i++;
			}
		}
		if (!found_id){
			if (mydata->nb_genome<MAXROBOT){
				mydata->nb_genome++;
			}else{
				i--;
			}
		}
		mydata->genome_list[i].id=ID;
		uint16_t fit=100*mydata->message.data[7]+mydata->message.data[8];
		uint16_t tmp=10000*mydata->message.data[1]+1000*mydata->message.data[2]+100*mydata->message.data[3]+10*mydata->message.data[4]+mydata->message.data[5];
		mydata->genome_list[i].proba=tmp;
		mydata->genome_list[i].fitness=fit;
		mydata->new_message=0;
}


void message_rx (message_t *message, distance_measurement_t *distance){

    mydata->new_message = 1;
		mydata->message=*message;
		// mydata->message_dist=estimate_distance(distance);
	//	mydata->toAggregate.dist = estimate_distance(distance);
		//if (kilo_uid==8) printf("DISTANCE.toAggregate : %d\n",mydata->toAggregate.dist);
}


void setup_message(){
	mydata->broadcast=0;//ne pas transmettre quand on change le message

	mydata->msg_transmis.type = NORMAL;
	mydata->msg_transmis.data[0] = kilo_uid; //on sait que le kilo_uid < 256 car on a pas autant de kilobot
	uint16_t tmp=mydata->proba;
	mydata->msg_transmis.data[1]=tmp/10000;
	tmp-=10000*(tmp/10000);
	mydata->msg_transmis.data[2]=tmp/1000;
	tmp-=1000*(tmp/1000);
	mydata->msg_transmis.data[3]=tmp/100;
	tmp-=100*(tmp/100);
	mydata->msg_transmis.data[4]=tmp/10;
	tmp-=10*(tmp/10);
	mydata->msg_transmis.data[5]=tmp;
	uint16_t fit=fitness();
	mydata->msg_transmis.data[8]=fit%100;
	mydata->msg_transmis.data[7]=fit/100;
	if(mydata->dead) mydata->msg_transmis.data[8]=255;
	mydata->msg_transmis.crc = message_crc(&mydata->msg_transmis);

	mydata->broadcast=1;
	return;
}



message_t *message_tx(){
	if(mydata->broadcast && !mydata->dead){
		return &(mydata->msg_transmis);
	}
	return 0;
}
