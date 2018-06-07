#include <kilombo.h>
#include <stdio.h>
#include "mEDEA.h"
#include "proba.h"
#include "communication.h"
#include "movement.h"

#include "stdlib.h"
#include <string.h>
/*
TODO :
Rajouter un timer en mode repelling ? (genre 10s pour eveiter de faire n imp)
Ajouter une fonction de proba qui donne une proba de leave en fonction du nombre de voisins que on a
Demander au prof stage
Demander au prof comment mettre le bloc actif dans cet algo. -idée : faire comme sin il avait déjà 5 voisins (variable ?)-
*/

//Idée de cahier des charges : mettre en avant l experimentation du bloc actif et de la modification des algos pour ces blocs actifs.





REGISTER_USERDATA(USERDATA)

void setup() {
	mydata->msg_transmis.type = NORMAL;
	mydata->msg_transmis.data[0] = kilo_uid;
	mydata->msg_transmis.data[8] =0;//on utilise cet octet pour savoir si le message est un génome ou information seulement 0=info 1=genome
	mydata->msg_transmis.crc = message_crc(&mydata->msg_transmis);

	mydata->toAggregate.nb_voisins=0;
	mydata->toAggregate.dist=-1;

	mydata->new_message = 0;
	mydata->last_dist_update = -1;
	mydata->toAggregate.dist = -1;
	mydata->state = SEARCHING;
	mydata->broadcast = 1;
	mydata->message_sent = 0;
	mydata->last_motion_update=0;
	mydata->nb_voisins=0;
	mydata->start_repelling=0;

	//mydata->curr_motion = STOP;
mydata->dead=0;
mydata->genome_setup=0;
mydata->nb_genome=0;
mydata->last_allowed=0;
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
mydata->parent=kilo_uid;

genome_alea();
	set_motion(STRAIGHT);


	mydata->time_update_fitness=0;
	// char nu[3];
	// snprintf(nu,3,"%d",kilo_uid);
	// // itoa(kilo_uid,nu,10);
	// char name[20]="stats_robot/";
	// strcat(name,nu);
	// // printf("%s\n",name );
	// // name=""
	//  mydata->fichier=fopen(name,"w");
	//  mydata->ecrire=1;
}

void genome_alea(){
	if (!mydata->genome_setup){ //la premiere fois : setup du genome aleatoirement
		int i;
		for (i=0;i<GENOMEPARAM;i++){ //initialisation du génome aléatoirement
			mydata->genome[i]=rand_hard()%3;
		}
		mydata->genome_setup=1;
		printf("%d genome set\n",kilo_uid );
		int j=0;
		for(;j<GENOMEPARAM;j++){
			printf("%d  ",mydata->genome[j] );
		}
		printf("\n");
	}else{
		if(!mydata->nb_genome){
			printf("%d DIED\n", kilo_uid);
			mydata->dead=1;
			mydata->last_genome_update=kilo_ticks;
			return;
		}
		int bestfit=mydata->genome_list[0].fitness;
		int best_indice=0;
		int i=0;
		for (i=0;i<mydata->nb_genome;i++){
			if (mydata->genome_list[i].fitness > bestfit){
				bestfit=mydata->genome_list[i].fitness;
				best_indice=i;

			}
		}
		// printf("%d best fit %d\n",kilo_uid,bestfit );
		i=0;
		for (i=0;i<GENOMEPARAM; i++){
			mydata->genome[i]=mydata->genome_list[best_indice].genome[i];
			// printf("%d ",mydata->genome[i] );
		}
		mydata->parent=mydata->genome_list[best_indice].parent;
		// printf("apres mutation\n" );

		//mutation
		if (((float)rand_hard())/256 < PROBA_MUTATION){
			int indice_change=rand_hard()/(256/GENOMEPARAM);
			int chang=(rand_hard()/128)+1;
			mydata->genome[indice_change]=(mydata->genome[indice_change]+chang)%3;

		}

		// for (i=0;i<GENOMEPARAM;i++){
		// 	printf("%d ",mydata->genome[i]);
		// }
		// printf("apres mutation\n" );
		/* SI CHOIX RANDOM PARMIS TOUS LES GENOMES */
		// int i=rand_hard()%mydata->nb_genome;
		// int j;
		// for (j=0;j<GENOMEPARAM;j++){//pas de mutation pour le moment
		// 	mydata->genome[j]=mydata->genome_list[i].genome[j];
		// }

	}
	mydata->last_genome_update=kilo_ticks;
	mydata->nb_genome=0;//reset des genomes recus
	setup_message();
}

int fitness(){
	return fitness2();
	int i;
	int fit=0;
	for (i=0;i<TIMEUPDATE;i++){
		fit+=mydata->last_fitness[i];
	}
	if (bienmange()){
		fit*=2;
	}
	if (mydata->dead==1){
		return 255;
	}
	// toRet/=TIMEUPDATE;
	// pri++){ntf("uid %d fit %d\n",kilo_uid,fit );
	return fit;
}

int fitness2(){//fitness d'aglomération
	int i=0;
	uint32_t fit=mydata->nb_voisins;
	for(;i<mydata->nb_voisins;i++){
			fit+=mydata->voisins_liste[i].nb_voisins;
			if(isfood(mydata->voisins_liste[i].id))
				fit=254;
	}
	if(fit>254) fit=254;
	return fit;
}

int bienmange(){
	int i;
	int fit=0;
	for (i=0;i<TIMEUPDATE;i++){
		fit+=mydata->last_fitness[i];
	}
	if (fit>TIMEUPDATE/2){
		return 1;
	}
	return 0;
}




void loop() {
	//52 loop par secondes sur simulateur
//TEST
	// if (kilo_ticks%(60*SECONDE)==0 && mydata->ecrire==1){
	// 	fprintf(mydata->fichier, "%d\n",mydata->dead );
	// 	// if(mydata->dead){
	// 	// 	fprintf(mydata->fichier, "%d\n",-1 );
	// 	// }else{
	// 	// 	fprintf(mydata->fichier, "%d\n",mydata->parent );
	// 	// }
	// 	// if(mydata->dead){
	// 	// 	fprintf(mydata->fichier, "%d\n",-1 );
	// 	// }else{
	// 	// 	fprintf(mydata->fichier, "%d%d%d%d%d%d%d%d\n",mydata->genome[0],mydata->genome[1],mydata->genome[2],mydata->genome[3],mydata->genome[4],mydata->genome[5],mydata->genome[6],mydata->genome[7] );
	// 	// }
	//
	// 	mydata->ecrire=0;
	// }
// if (kilo_ticks%SECONDE==1){
// 	mydata->ecrire=1;
// }

	if (isfood(kilo_uid)){
	set_color(RGB(3,3,0));
	 	emission();
		return;
	}
	if(kilo_ticks>mydata->time_update_fitness+SECONDE/2){
		update_fitness();
		mydata->time_update_fitness=kilo_ticks;
		// printf("%d \n",mydata->last_update_fitness );
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

	//printf("uid : %d last motion update %d \n",kilo_uid,mydata->last_motion_update);
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

int isfood(int idrobot){
	uint8_t i=0;
	int food[NBFOOD] = {IDFOOD};
	for(;i<NBFOOD;i++){
		if(idrobot==food[i])
			return 1;
	}
	return 0;
}


int main() {
	/*
Initialise callback et lance la main loop
*/
    kilo_init();
    kilo_message_rx = message_rx;
    kilo_message_tx = message_tx;
		kilo_message_tx_success = message_tx_success;

		SET_CALLBACK(obstacles, callback_obstacles);
		SET_CALLBACK(botinfo, botinfo);
		// SET_CALLBACK(json_state, json_state);



		kilo_start(setup, loop);

    return 0;
}
#ifdef SIMULATOR


void do_stats(char * path){

}


static char botinfo_buffer[10000000];
// provide a text string for the status bar, about this bot
char *botinfo(void)
{
  int i;
  char *p = botinfo_buffer;
  p+= sprintf (p, "ID: %d \n", kilo_uid);

  // p+= sprintf (p, "move: %d wait:%d turn:%d\n", mydata->move, mydata->wait, mydata->turn);
  p+= sprintf (p, "genome: [%d,%d,%d,%d,%d,%d,%d,%d]  fitness: %d parent:%d ",
	       mydata->genome[0],mydata->genome[1],mydata->genome[2],mydata->genome[3],mydata->genome[4],mydata->genome[5],mydata->genome[6],mydata->genome[7],fitness(),mydata->parent );

  p += sprintf (p, "%d neighbors: \n", mydata->nb_voisins);
	p+= sprintf(p,"%d genomes : ",mydata->nb_genome);
  for (i = 0; i < mydata->nb_genome; i++)
    //    p += sprintf (p, "%d ", mydata->neighbors[i].ID);
    p += sprintf (p, "[id : %d,fit : %d]", mydata->genome_list[i].id, mydata->genome_list[i].fitness);


  return botinfo_buffer;
}

#define MUR 1500

int16_t callback_obstacles(double x, double y, double *m1, double *m2){
    if (x > MUR || x< -MUR || y>MUR|| y<-MUR){
      if(x>MUR || x<-MUR){
        *m1 = (x<0)? 1:-1;
      }
      if (y>MUR || y<-MUR){
        *m2 = (y<0)? 1:-1;
      }

      return 1;
    }

    return 0;
}

json_t *json_state(){
	fclose(mydata->fichier);
    //create the state object we return
    json_t* state = json_object();
//stats genome
    // char content[13];
		// int i=0;
		// for(i=0;i<8;i++){
		// 	switch(mydata->genome[i]){
		// 		case 0:
		// 		content[i]='-1';
		// 		break;
		// 		case 1:
		// 		content[i]='0';
		// 		break;
		// 		case 2:
		// 		content[i]='1';
		// 		break;
		// 	}
		// }
		//  content[8]='_';

		//stats loss
		char *content;
  	if (mydata->dead==1){
			content="SEARCHING";
		}else{
			content="SLEEPING";
		}
    json_t* g = json_string(content);
    json_object_set (state, "states", g);

    return state;
}

#endif
