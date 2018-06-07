#include <kilombo.h>
#include <stdio.h>
#include "proba.h"
#include "mEDEA.h"
#include "communication.h"
#include "movement.h"

extern USERDATA * mydata;

void update_voisins(){
	if (!mydata->nb_voisins){
		return;
	}
	int i;
	for(i=mydata->nb_voisins-1;i>=0;i--){
		if (kilo_ticks-mydata->voisins_liste[i].timestamp > SECONDE){
		// if (kilo_uid==83)	printf("delete %d\n",mydata->voisins_liste[i].id );
			mydata->voisins_liste[i]=mydata->voisins_liste[mydata->nb_voisins];
			mydata->voisins_liste[mydata->nb_voisins-1].id=-1;

			mydata->nb_voisins--;
		}
	}
}

int is_feed(){
	int i;
	for( i=0;i<mydata->nb_voisins;i++){
		if(isfood(mydata->voisins_liste[i].id)){
			return 1;
		}
		// if(mydata->voisins_liste[i].id==IDFOOD){
		// 	return 1;
		// }
	}
	return 0;
}

void emission(){
	// Blink the LED magenta whenever a message is sent.
	if (mydata->message_sent == 1)
	{
		//printf("EMISSION %d\n",kilo_uid );
		mydata->message_sent = 0;
		set_color(RGB(0, 0, 1));
			if (kilo_ticks>mydata->last_dist_update+(SECONDE/5)){
				mydata->last_dist_update=kilo_ticks;

			}
	}
}
void update_only_voisins_from_message(){
	/*update de la distance
	*/
	uint8_t found_id=0;
	uint32_t distance=mydata->message_dist;
	int ID=mydata->message.data[0];
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

	// if (kilo_uid==83) printf("%d %d nb %d\n",message->data[0],distance ,mydata->nb_voisins);

	mydata->new_message=0;
}

void update_from_message(){
	/*update de la distance
	*/
	// if (!mydata->message.data[8] || mydata->message.data[0]==IDFOOD){ //on se fout du génome du bloc actif
	// 	update_only_voisins_from_message();
	// 	return;
	// }
	if (mydata->message.data[8]==255) return; //255 correspond à un kilobot dead (voir fonction fitness dans mEDEA.c)
	uint8_t found_id=0;
	uint32_t distance=mydata->message_dist;
	int ID=mydata->message.data[0];
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


/*update du génome de ce voisin
*/
	// if (ID==IDFOOD) return;
	if(isfood(ID)) return;
	if (mydata->message.data[8]==255) return; //255 correspond à un kilobot dead (voir fonction fitness dans mEDEA.c)
	found_id=0;
	i=0;
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
			// int j;
			// for (int k=0;k<9;k++){
			// 	printf("%d ",mydata->message.data[k] );
			// }
			// printf("message recu\n" );
			mydata->genome_list[i].id=ID;

			mydata->genome_list[i].genome[0]=mydata->message.data[1]/100;
			mydata->message.data[1]-=mydata->genome_list[i].genome[0]*100;
			// printf("extraction %d nouveau %d\n",mydata->genome_list[i].genome[0],mydata->message.data[1] );

			mydata->genome_list[i].genome[1]=mydata->message.data[1]/10;
			mydata->message.data[1]-=mydata->genome_list[i].genome[1]*10;
			// printf("extraction %d nouveau %d\n",mydata->genome_list[i].genome[1],mydata->message.data[1] );

			mydata->genome_list[i].genome[2]=mydata->message.data[1];
			// printf("extraction %d nouveau-pasnouveauici %d\n",mydata->genome_list[i].genome[2],mydata->message.data[1] );


			mydata->genome_list[i].genome[3]=mydata->message.data[2]/100;
			mydata->message.data[2]-=mydata->genome_list[i].genome[3]*100;
			// printf("extraction %d nouveau %d\n",mydata->genome_list[i].genome[3],mydata->message.data[2] );

			mydata->genome_list[i].genome[4]=mydata->message.data[2]/10;
			mydata->message.data[2]-=mydata->genome_list[i].genome[4]*10;

			mydata->genome_list[i].genome[5]=mydata->message.data[2];


			mydata->genome_list[i].genome[6]=mydata->message.data[3]/100;
			mydata->message.data[3]-=mydata->genome_list[i].genome[6]*100;

			mydata->genome_list[i].genome[7]=mydata->message.data[3]/10;

			mydata->genome_list[i].fitness=mydata->message.data[8];
			// printf("fitness recu : %d\n",mydata->genome_list[i].fitness );

			mydata->genome_list[i].parent=mydata->message.data[7]; //parent du génome actuel
			// for(j=0;j<GENOMEPARAM;j++){
			// 	printf("%d ",mydata->genome_list[i].genome[j]);
			// }
			// printf("fin du genome du voisin\n\n");


	// if (kilo_uid==83) printf("%d %d nb %d\n",message->data[0],distance ,mydata->nb_voisins);

	mydata->new_message=0;
}

void update_fitness(){
	int i;
	for(i=0;i<mydata->nb_voisins;i++){
		if (isfood(mydata->voisins_liste[i].id)){
			mydata->last_fitness[mydata->last_update_fitness]=1;
			mydata->last_update_fitness=(mydata->last_update_fitness+1)%TIMEUPDATE;
			return;

		}
	}
	mydata->last_fitness[mydata->last_update_fitness]=0;
	mydata->last_update_fitness=(mydata->last_update_fitness+1)%TIMEUPDATE;
	return;
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
	// for(i=0;i<GENOMEPARAM;i++){
	// 	mydata->msg_transmis.data[i+1]=mydata->genome[i];
	// }
	//genome est entre {-1,0,1}:
	int i=0;
	for(i=0;i<GENOMEPARAM;i++){
		printf("%d ",mydata->genome[i] );
	}
	printf("genome mydata\n" );
	int tmp1=100*(mydata->genome[0])+10*(mydata->genome[1])+mydata->genome[2];
	int tmp2=100*(mydata->genome[3])+10*(mydata->genome[4])+mydata->genome[5];
	int tmp3=100*(mydata->genome[6])+10*(mydata->genome[7]);
	mydata->msg_transmis.data[1]=tmp1;//transmition genome
	mydata->msg_transmis.data[2]=tmp2;//transmition genome
	mydata->msg_transmis.data[3]=tmp3;//transmition genome
	mydata->msg_transmis.data[7]=mydata->parent;//transmition parent
	mydata->msg_transmis.data[8]=fitness();//transmition fitness

	i=0;
	for(i=0;i<9;i++){
		printf("%d ",mydata->msg_transmis.data[i]);
	}
	printf("genome transmis\n" );
	mydata->msg_transmis.crc = message_crc(&mydata->msg_transmis);


	mydata->broadcast=1;
	return;
}

void setup_message_fitness(){
	mydata->broadcast=0;//ne pas transmettre quand on change le message

	mydata->msg_transmis.data[8]=fitness();
	// if(kilo_uid!=IDFOOD) printf("fit setup message %d\n",mydata->msg_transmis.data[8] );
	mydata->msg_transmis.crc = message_crc(&mydata->msg_transmis);

	mydata->broadcast=1;

	return;
}

void share_genome_allowed(){
	mydata->broadcast=0;//ne pas transmettre quand on change le message

	mydata->msg_transmis.type = NORMAL;
	mydata->msg_transmis.data[0] = kilo_uid; //on sait que le kilo_uid < 256 car on a pas autant de kilobot
int i;
	for(i=0;i<GENOMEPARAM;i++){
		mydata->msg_transmis.data[i+1]=mydata->genome[i];
	}
	mydata->msg_transmis.data[8]=1;
	mydata->msg_transmis.crc = message_crc(&mydata->msg_transmis);


	mydata->broadcast=1;
	return;
}

void share_genome_forbiden(){
	mydata->broadcast=0;//ne pas transmettre quand on change le message

	mydata->msg_transmis.type = NORMAL;
	mydata->msg_transmis.data[0] = kilo_uid; //on sait que le kilo_uid < 256 car on a pas autant de kilobot
int i;
	for(i=0;i<GENOMEPARAM;i++){
		mydata->msg_transmis.data[i+1]=mydata->genome[i];
	}
	mydata->msg_transmis.data[8]=0;
	mydata->msg_transmis.crc = message_crc(&mydata->msg_transmis);


	mydata->broadcast=1;
	return;
}

message_t *message_tx(){
	if (kilo_uid==0){
		return &mydata->msg_transmis;
	}
	if(mydata->broadcast){
		 //vérification de si il est proche du bloc qui permet de broadcast-peut changer toutes les 2 secondes
		//printf("%d test\n",kilo_uid );
		return &(mydata->msg_transmis);

	} else {
  	return 0;
	}
	return 0;
	//return &(mydata->message);
}

void message_tx_success()
{
    // Set the flag on message transmission.
    mydata->message_sent = 1;
}
