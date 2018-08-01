#include <kilombo.h>
#include <stdio.h>
#include <math.h>
#include "mEDEA.h"
/*TODO:
remplacer une des entrées du perceptron par la distance au mur (= à 255 si pas de mur en vue)
include mur dans la fitness ??
	-> a mon avis pas pour le moment


->mettre à jour les communications pour intergrer le mur.
		*voir dans aggregation_proba

*/


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
	//MUR
	mydata->mur_dist=-1;
	mydata->last_mur_dist=-1;
	mydata->mur_update=0;
	mydata->last_light=500;
	int i=0;
	for(i=0;i<TIMEUPDATE;i++){
		mydata->last_fitness[i]=0;
	}
	mydata->last_update_fitness=0;
	i=0;
	for(i=1;i<9;i++){
		mydata->msg_transmis.data[i]=0;
	}
	for(i=0;i<11;i++){
		mydata->genome[i]=0;
	}
	genome_alea();
	spinup_motors();
	set_motors(kilo_straight_left, kilo_straight_right);
	mydata->time_update_fitness=0;
}

void genome_alea(){
	int color = 0;
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
		// color = 0;
		// int i=0;
		// for (i=0;i<mydata->nb_genome;i++){
		// 	if (mydata->genome_list[i].fitness > bestfit){
		// 		bestfit=mydata->genome_list[i].fitness;
		// 		best_indice=i;
		// 	}
		// }
		best_indice=rand_hard()%mydata->nb_genome;
		int i=0;
		for (i=0;i<GENOMEPARAM; i++){
			mydata->genome[i]=mydata->genome_list[best_indice].genome[i];
			// printf("%d ",mydata->genome[i] );
		}

		//mutation
		if (((float)rand_hard())/256 < PROBA_MUTATION){
			int indice_change=rand_hard()/(256/GENOMEPARAM);
			int chang=(rand_hard()/128)+1;
			mydata->genome[indice_change]=(mydata->genome[indice_change]+chang)%3;

		}
	}
	//Coloration de la LED en fonction du génome
	uint8_t i=0;
	for (i=0;i<GENOMEPARAM; i++){
	color += mydata->genome[i]*pow(3,i);
}
	set_color(color);


	mydata->last_genome_update=kilo_ticks;
	mydata->nb_genome=0;//reset des genomes recus
	setup_message();
}

uint16_t fitness(){
	return is_feed();
}




void loop() {
	//52 loop par secondes sur simulateur
	// if (kilo_uid==IDFOOD){
	// set_color(RGB(1,0,0));
	//  	emission();
	// 	return;
	// }
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
	if(kilo_ticks > mydata->last_genome_update + SECONDE){
		if(!mydata->dead){
				//set_color(RGB(0,1,0));
		}

	}
	if (kilo_ticks > mydata->last_genome_update + 60 * SECONDE){
		set_color(RGB(3,3,3));
			genome_alea(); //choisir génome aléatoirement

	}



	if (kilo_ticks > mydata->last_motion_update + SECONDE){
		mydata->last_motion_update = kilo_ticks;
		if (mydata->dead){
			set_color(RGB(0,0,0));
			set_motors(0,0);
		}else{
			genome_motion();//CONTROLLEUR
			// printf("%d%d%d%d%d%d%d%d\n",mydata->genome[0],mydata->genome[1],mydata->genome[2],mydata->genome[3],mydata->genome[4],mydata->genome[5],mydata->genome[6],mydata->genome[7] );
		}
	}
}

/** ** ** MOVEMENT ** ** **/

// void set_motion(uint8_t motion){
// 	if(mydata->curr_motion != motion){
// 		switch(motion){
// 			case LEFT:
// 				spinup_motors();
// 				set_motors(kilo_turn_left, 0);
// 				break;
// 			case RIGHT:
// 				spinup_motors();
// 				set_motors(0, kilo_turn_right);
// 				break;
// 			case STRAIGHT:
// 				spinup_motors();
// 				set_motors(kilo_straight_left, kilo_straight_right);
// 				break;
// 			case STOP:
// 				default:
// 				set_motors(0, 0);
// 			break;
// 		}
// 		mydata->curr_motion = motion;
// 	}
// }

void genome_motion(){//controlleur
	uint32_t moyenne=0; //distance moyenne des voisins
	int i;
	if (mydata->nb_voisins){
		for (i=0;i<mydata->nb_voisins;i++){
			moyenne+=mydata->voisins_liste[i].dist;
		}
		moyenne/=mydata->nb_voisins;
	}
	// double moteur_g=1.0*(mydata->genome[0]-1)+1.0*(mydata->genome[2]-1)*moyenne+1.0*(mydata->genome[4]-1)*mydata->nb_voisins+(mydata->genome[6]-1)*is_feed(); //biais + moyenne_distance_des_voisin + nb_voisins
	// double moteur_d=1.0*(mydata->genome[1]-1)+1.0*(mydata->genome[3]-1)*moyenne+1.0*(mydata->genome[5]-1)*mydata->nb_voisins+(mydata->genome[7]-1)*is_feed(); //biais + moyenne_distance_des_voisin + nb_voisins
	double moteur_g=1.0*(mydata->genome[0]-1)+1.0*(mydata->genome[2]-1)*is_feed();//+1.0*(mydata->genome[4]-1)*mydata->nb_voisins+(mydata->genome[6]-1)*is_feed()+(mydata->genome[8]-1)*hasMur(); //biais + moyenne_distance_des_voisin + nb_voisins
	double moteur_d=1.0*(mydata->genome[1]-1)+1.0*(mydata->genome[3]-1)*is_feed();//+1.0*(mydata->genome[5]-1)*mydata->nb_voisins+(mydata->genome[7]-1)*is_feed()+(mydata->genome[9]-1)*hasMur(); //biais + moyenne_distance_des_voisin + nb_voisins

	moteur_g=1.0/(1.0+exp(-moteur_g));
	moteur_d=1.0/(1.0+exp(-moteur_d));
	int real_g=arrondi((kilo_turn_left-(kilo_turn_left-15))*moteur_g)+(kilo_turn_left-15); //60 pour la vitesse minimale
	int real_d=arrondi((kilo_turn_right-(kilo_turn_right-15))*moteur_d)+(kilo_turn_right-15);
	//spinup_motors();

	// if((real_d-real_g)>7){
	// 	real_g=real_d-7;
	// }else if((real_g-real_d)>7){
	// 	real_d=real_g-7;
	// }

	mydata->moteur_d=real_d;
	mydata->moteur_g=real_g;

	//printf("KTL : %d, KTR : %d\n", kilo_turn_left, kilo_turn_right);
	//printf("MOTOR_G : %d  MOTOR_D : %d\n",real_g, real_d);
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
	uint8_t found_id=0;
	int ID=mydata->message.data[0];
	if(isMur(ID)) return update_mur_from_message();
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
		/*
		mydata->genome_list[i].genome[4]=mydata->message.data[2]/10;
		mydata->message.data[2]-=mydata->genome_list[i].genome[4]*10;
		mydata->genome_list[i].genome[5]=mydata->message.data[2];
		mydata->genome_list[i].genome[6]=mydata->message.data[3]/100;
		mydata->message.data[3]-=mydata->genome_list[i].genome[6]*100;
		mydata->genome_list[i].genome[7]=mydata->message.data[3]/10;
		mydata->message.data[3]-=mydata->genome_list[i].genome[7]*10;
		mydata->genome_list[i].genome[8]=mydata->message.data[3];
		mydata->genome_list[i].genome[9]=mydata->message.data[4]/100;
		*/

		mydata->genome_list[i].fitness=mydata->message.data[8];

		mydata->genome_list[i].fitness=0;
		mydata->new_message=0;
}
//WORK
void setup_message_fitness(){
	mydata->broadcast=0;//ne pas transmettre quand on change le message
	uint16_t fit=fitness();

	mydata->msg_transmis.data[8]=fit%100;
	mydata->msg_transmis.data[7]=fit/100;
	if(mydata->dead) mydata->msg_transmis.data[8]=255;
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
	int tmp2=100*(mydata->genome[3]);
	mydata->msg_transmis.data[1]=tmp1;//transmission genome
	mydata->msg_transmis.data[2]=tmp2;//transmission genome
	mydata->msg_transmis.data[8]=fitness();//transmition fitness
	mydata->msg_transmis.crc = message_crc(&mydata->msg_transmis);
	mydata->broadcast=1;
	return;
}

uint16_t is_feed(){
	uint16_t toRet=get_ambientlight();
	while(toRet==-1){
		toRet=get_ambientlight();
	}
	return toRet;
}

uint8_t hasMur(){
	//return le distance au mur si celui ci est présent
	//sinon return -1
  if(kilo_ticks>mydata->mur_update+SECONDE) return -1;
  return mydata->mur_dist;
}


int arrondi(double n){
	return n+0.5;
}

// void emission(){
// 	// Blink the LED magenta whenever a message is sent.
// 	if (mydata->message_sent == 1)
// 	{
// 		mydata->message_sent = 0;
// 		set_color(RGB(0, 0, 1));
// 	}
// }


int main() {
	/*
Initialise callback et lance la main loop
*/
    kilo_init();
		#ifdef DEBUG
		// setup debugging, i.e. printf to serial port, in real Kilobot
		debug_init();
		#endif
    kilo_message_rx = message_rx;
    kilo_message_tx = message_tx;
		kilo_message_tx_success = message_tx_success;

		SET_CALLBACK(botinfo, botinfo);
		SET_CALLBACK(obstacles, callback_obstacles);

		kilo_start(setup, loop);
    return 0;
}



#ifdef SIMULATOR

static char botinfo_buffer[10000000];
// provide a text string for the status bar, about this bot
char *botinfo(void)
{
  int i;
  char *p = botinfo_buffer;
  p+= sprintf (p, "ID: %d fit : %d light : %d\n", kilo_uid,fitness(),get_ambientlight());

  // p+= sprintf (p, "move: %d wait:%d turn:%d\n", mydata->move, mydata->wait, mydata->turn);

	if(mydata->dead){
		p+=sprintf(p," DEAD \n");
		return botinfo_buffer;
	}
  p+= sprintf (p, "genome: [%d,%d,%d,%d,%d,%d,%d,%d,%d,%d], m_g %d m_d %d \n\n",
	       mydata->genome[0],mydata->genome[1],mydata->genome[2],mydata->genome[3],mydata->genome[4],mydata->genome[5],mydata->genome[6],mydata->genome[7],mydata->genome[8],mydata->genome[8],mydata->moteur_g,mydata->moteur_d);

  p += sprintf (p, "%d neighbors: ", mydata->nb_voisins);
	for(i=0; i<mydata->nb_voisins;i++)
			p+=sprintf(p, "[id : %d, dist : %d, time : %d ]\n",mydata->voisins_liste[i].id,mydata->voisins_liste[i].dist,kilo_ticks-mydata->voisins_liste[i].timestamp);

	p+= sprintf(p,"\n%d genomes : ",mydata->nb_genome);
  for (i = 0; i < mydata->nb_genome; i++)
    //    p += sprintf (p, "%d ", mydata->neighbors[i].ID);
    p += sprintf (p, "[id : %d,fit : %d]\n ", mydata->genome_list[i].id, mydata->genome_list[i].fitness);

  return botinfo_buffer;
}

#define MUR 500

int16_t callback_obstacles(double x, double y, double *m1, double *m2){

	//CERCLE
	if(x*x + y*y >= MUR*MUR){
			*m1 = (x<0)? 1:-1;
			*m2 =  (y<0)? 1:-1;
			return 1;
	}
	else{
			return 0;
	}

//CARRE

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

#endif
