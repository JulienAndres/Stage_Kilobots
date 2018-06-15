#include <kilombo.h>
#include <stdio.h>
#include <math.h>
#include "proba.h"
#include "mEDEA.h"
#include "communication.h"
#include "movement.h"

extern USERDATA * mydata;

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

void motion_dead(){
		set_motors(0, 0);
}

void set_random_direction(){
    uint8_t random_number = rand_hard()%4;
    //Compute the remainder of random_number when divided by 3.
    // This gives a new random number in the set {0, 1, 2}. The value is incremented and returned.
    if ((random_number == 0) || (random_number == 3)){
				set_motion(STRAIGHT);
    }
    else if(random_number == 1){
				set_motion(LEFT);
    }
    else{
				set_motion(RIGHT);
    }
}

void set_random_turning_direction(){
	//printf("ici N?\n" );
	uint8_t random_number = rand_hard()%2;
	if(random_number == 0){
		set_motion(LEFT);
	} else {
		set_motion(RIGHT);
	}
}




void genome_motion(){//controlleur
	// set_color(RGB(0,1,0));
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


	int real_g=arrondi((kilo_turn_left-(kilo_turn_left-20))*moteur_g)+(kilo_turn_left-20); //60 pour la vitesse minimale
	int real_d=arrondi((kilo_turn_right-(kilo_turn_right-20))*moteur_d)+(kilo_turn_right-20);
	 // printf(" reeal gauche %d droit %d\n",real_g, real_d);
	spinup_motors();
	set_motors(real_g, real_d);

	return;
	}
