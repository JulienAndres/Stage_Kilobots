#include <kilombo.h>
#include <stdio.h>
#include <math.h>
#include "mEDEA.h"

#include "stdlib.h"
#include <string.h>

REGISTER_USERDATA(USERDATA)

void setup() {
	mydata->msg_transmis.type = NORMAL;
	mydata->msg_transmis.data[0] = kilo_uid;
	mydata->msg_transmis.data[8] =0;//on utilise cet octet pour savoir si le message est un génome ou information seulement 0=info 1=genome
	mydata->msg_transmis.crc = message_crc(&mydata->msg_transmis);
	mydata->new_message = 0;
	mydata->broadcast = 1;
	mydata->message_sent = 0;
	mydata->last_motion_update=0;
	mydata->nb_voisins=0;
	mydata->dead=0;
	mydata->genome_setup=0;
	mydata->nb_genome=0;
	mydata->last_genome_update=0;
	int i=0;
	for(i=0;i<TIMEUPDATE;i++){
		mydata->last_fitness[i]=0;
	}
	mydata->last_update_fitness=0;
	i=0;
	for(i=1;i<9;i++){
		mydata->msg_transmis.data[i]=0;
	}
	genome_alea();
	set_motion(STRAIGHT);
	mydata->time_update_fitness=0;
}

void genome_alea(){
	if (!mydata->genome_setup){ //la premiere fois : setup du genome aleatoirement
		int i;
		for (i=0;i<GENOMEPARAM;i++){ //initialisation du génome aléatoirement
			mydata->genome[i]=rand_hard()%3;
		}
		mydata->genome_setup=1;
	}else{
		if(!mydata->nb_genome){
			mydata->dead=1;
			mydata->last_genome_update=kilo_ticks;
			return;
		}
		float bestfit=mydata->genome_list[0].fitness;
		int best_indice=0;
		int i=0;
		for (i=0;i<mydata->nb_genome;i++){
			if (mydata->genome_list[i].fitness > bestfit){
				bestfit=mydata->genome_list[i].fitness;
				best_indice=i;
			}
		}

		i=0;
		for (i=0;i<GENOMEPARAM; i++){
			mydata->genome[i]=mydata->genome_list[best_indice].genome[i];
			// printf("%d ",mydata->genome[i] );
		}

		if(kilo_uid == 1){
			printf("Changed genome");
		}

		//mutation
		if (((float)rand_hard())/256 < PROBA_MUTATION){
			int indice_change=rand_hard()/(256/GENOMEPARAM);
			int chang=(rand_hard()/128)+1;
			mydata->genome[indice_change]=(mydata->genome[indice_change]+chang)%3;

		}
	}
	mydata->last_genome_update=kilo_ticks;
	mydata->nb_genome=0;//reset des genomes recus
	setup_message();
}

int fitness(){
	int i;
	int fit=0;
	for (i=0;i<TIMEUPDATE;i++){
		fit+=mydata->last_fitness[i];
	}
	if (mydata->dead==1){
		return 255;
	}
	return fit;
}




void loop() {
	//52 loop par secondes sur simulateur
	if (kilo_uid==IDFOOD){
	set_color(RGB(1,0,0));
	 	emission();
		return;
	}
	if(kilo_ticks>mydata->time_update_fitness+SECONDE){
		update_fitness();
		mydata->time_update_fitness=kilo_ticks;
	}
	update_voisins();
	// update_fitness();
	setup_message_fitness();
	if(mydata->new_message==1){
		update_from_message();
	}
	if (kilo_ticks > mydata->last_genome_update + 60 * SECONDE){
			genome_alea(); //choisir génome aléatoirement
	}
	if (kilo_ticks > mydata->last_motion_update + SECONDE){
		mydata->last_motion_update = kilo_ticks;
		if (mydata->dead){
			set_color(RGB(0,0,0));
			motion_dead();
		}else{
			genome_motion();//CONTROLLEUR
		}
	}
}

/** ** ** MOVEMENT ** ** **/

void set_motion(uint8_t motion){
	if(mydata->curr_motion != motion){
		switch(motion){
			case LEFT:
				spinup_motors();
				set_motors(kilo_turn_left, 0);
				break;
			case RIGHT:
				spinup_motors();
				set_motors(0, kilo_turn_right);
				break;
			case STRAIGHT:
				spinup_motors();
				set_motors(kilo_straight_left, kilo_straight_right);
				break;
			case STOP:
				default:
				set_motors(0, 0);
			break;
		}
		mydata->curr_motion = motion;
	}
}

void genome_motion(){//controlleur
	set_color(RGB(0,1,0));
	uint32_t moyenne=0; //distance moyenne des voisins
	int i;
	if (mydata->nb_voisins){
		for (i=0;i<mydata->nb_voisins;i++){
			moyenne+=mydata->voisins_liste[i].dist;
		}
		moyenne/=mydata->nb_voisins;
	}

	double moteur_g=1.0*(mydata->genome[0]-1)+1.0*(mydata->genome[2]-1)*moyenne+1.0*(mydata->genome[4]-1)*mydata->nb_voisins+(mydata->genome[6]-1)*is_feed(); //biais + moyenne_distance_des_voisin + nb_voisins
	double moteur_d=1.0*(mydata->genome[1]-1)+1.0*(mydata->genome[3]-1)*moyenne+1.0*(mydata->genome[5]-1)*mydata->nb_voisins+(mydata->genome[7]-1)*is_feed(); //biais + moyenne_distance_des_voisin + nb_voisins
	moteur_g=1.0/(1.0+exp(-moteur_g));
	moteur_d=1.0/(1.0+exp(-moteur_d));
	int real_g=arrondi((kilo_turn_left-60)*moteur_g)+60; //60 pour la vitesse minimale
	int real_d=arrondi((kilo_turn_right-60)*moteur_d)+60;
	spinup_motors();
	set_motors(real_g, real_d);
	return;
	}

/** ** ** -------- ** ** **/

/** COMMUNICATION **/

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

void update_from_message(){
	uint8_t found_id=0;
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
	mydata->voisins_liste[i].dist=mydata->message_dist;
/*update du génome de ce voisin */
	if (ID==IDFOOD) return;
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
		mydata->genome_list[i].id=ID;
		mydata->genome_list[i].genome[0]=mydata->message.data[1]/100;
		mydata->message.data[1]-=mydata->genome_list[i].genome[0]*100;
		mydata->genome_list[i].genome[1]=mydata->message.data[1]/10;
		mydata->message.data[1]-=mydata->genome_list[i].genome[1]*10;
		mydata->genome_list[i].genome[2]=mydata->message.data[1];
		mydata->genome_list[i].genome[3]=mydata->message.data[2]/100;
		mydata->message.data[2]-=mydata->genome_list[i].genome[3]*100;
		mydata->genome_list[i].genome[4]=mydata->message.data[2]/10;
		mydata->message.data[2]-=mydata->genome_list[i].genome[4]*10;
		mydata->genome_list[i].genome[5]=mydata->message.data[2];
		mydata->genome_list[i].genome[6]=mydata->message.data[3]/100;
		mydata->message.data[3]-=mydata->genome_list[i].genome[6]*100;
		mydata->genome_list[i].genome[7]=mydata->message.data[3]/10;
		mydata->genome_list[i].fitness=mydata->message.data[8];
		mydata->new_message=0;
}

void setup_message_fitness(){
	mydata->broadcast=0;//ne pas transmettre quand on change le message
	mydata->msg_transmis.data[8]=fitness();
	mydata->msg_transmis.crc = message_crc(&mydata->msg_transmis);
	mydata->broadcast=1;
	return;
}

void update_fitness(){
	int i;
	for(i=0;i<mydata->nb_voisins;i++){
		if (mydata->voisins_liste[i].id==IDFOOD){
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
}

message_t *message_tx(){
	if (kilo_uid==0){
		return &mydata->msg_transmis;
	}
	if(mydata->broadcast){
		 //vérification de si il est proche du bloc qui permet de broadcast-peut changer toutes les 2 secondes
		return &(mydata->msg_transmis);
	} else {
  	return 0;
	}
	return 0;
}

void message_tx_success()
{
    // Set the flag on message transmission.
    mydata->message_sent = 1;
}

void setup_message(){
	mydata->broadcast=0;//ne pas transmettre quand on change le message
	mydata->msg_transmis.type = NORMAL;
	mydata->msg_transmis.data[0] = kilo_uid; //on sait que le kilo_uid < 256 car on a pas autant de kilobot
	//genome est entre {-1,0,1}:
	int tmp1=100*(mydata->genome[0])+10*(mydata->genome[1])+mydata->genome[2];
	int tmp2=100*(mydata->genome[3])+10*(mydata->genome[4])+mydata->genome[5];
	int tmp3=100*(mydata->genome[6])+10*(mydata->genome[7]);
	mydata->msg_transmis.data[1]=tmp1;//transmission genome
	mydata->msg_transmis.data[2]=tmp2;//transmission genome
	mydata->msg_transmis.data[3]=tmp3;//transmission genome
	mydata->msg_transmis.data[8]=fitness();//transmition fitness
	mydata->msg_transmis.crc = message_crc(&mydata->msg_transmis);
	mydata->broadcast=1;
	return;
}

int is_feed(){
	int i;
	for( i=0;i<mydata->nb_voisins;i++){
		if(mydata->voisins_liste[i].id==IDFOOD){
			return 1;
		}
	}
	return 0;
}


int arrondi(double n){
	return n+0.5;
}

void emission(){
	// Blink the LED magenta whenever a message is sent.
	if (mydata->message_sent == 1)
	{
		mydata->message_sent = 0;
		set_color(RGB(0, 0, 1));
	}
}

void motion_dead(){
		set_motors(0, 0);
}


int main() {
	/*
Initialise callback et lance la main loop
*/
    kilo_init();
    kilo_message_rx = message_rx;
    kilo_message_tx = message_tx;
		kilo_message_tx_success = message_tx_success;
		kilo_start(setup, loop);
    return 0;
}
