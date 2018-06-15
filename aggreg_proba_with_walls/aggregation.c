#include <kilombo.h>

#include "aggregation.h"
#include "movement.h"
#include "communication.h"


REGISTER_USERDATA(USERDATA)

void setup() {
  //NEIGHBORS
  mydata->nb_voisins=0;

  //COMMUNICATION
  mydata->broadcast=1;
  mydata->new_message=0;
  mydata->message_dist=-1;
  setup_message();

  //MUR
  mydata->mur_dist=-1;
  mydata->last_mur_dist=-1;
  mydata->mur_update=0;

  //MOVEMENT
  mydata->last_motion_update=kilo_ticks;
  set_random_direction();
  mydata->delai=1;

  return;
}

void loop(){
  if(kilo_uid==0){
    set_motion(STOP);
    set_color(RGB(0, 0, 1));
    return;
  }
  update_voisins();
  if(mydata->new_message) update_from_message();
  setup_message();

  if(kilo_ticks>mydata->last_motion_update+mydata->delai*SECONDE){
    mydata->last_motion_update=kilo_ticks;
    evitement_obstacle();
  }
  return;
}

void plan_manager(){
  return;
}

void converging(){

}

void evitement_obstacle(){
  /* Evitement mur
    sinon marche random */
  uint8_t mur=hasMur();
  if(mur){
    if(mur<=mydata->last_mur_dist){
      mydata->delai+=1;
      set_color(RGB(3,3,3));
      switch (mydata->curr_motion) {
        case RIGHT:
          set_motion(LEFT);
          break;
        case LEFT:
          set_motion(RIGHT);
          break;
        default:
          set_random_turning_direction();
          break;
      }
    }else{
      mydata->delai=1;
    }
    mydata->last_mur_dist=mur;
  }else{
    mydata->delai=1;
    set_color(RGB(0,3,0));
    set_random_direction();
    mydata->last_mur_dist=-1;
  }
}


uint8_t hasMur(){
	//return le distance au mur si celui ci est présent
	//sinon return 0
  if(kilo_ticks>mydata->mur_update+SECONDE) return 0;
  if(mydata->mur_dist>DISTOBSTACLE) return 0;
  return mydata->mur_dist;
}

uint8_t isMur(int ID){
  return ID==IDOBSTACLE;
}

uint8_t is_too_close(){
  /*
  return 1 si un des robots est assez proche
  return 0 si aucun robot n'est assez proche.
  */
	int i=0;
	for (;i<mydata->nb_voisins;i++){
		if (mydata->voisins_liste[i].dist <= DIST_TO_AGGREGATE){
			return 1;
		}
	}
	return 0;
}

uint8_t hasBestNeighbor(){
//A REVOIR
/*
  Cherche un robot auquel d'agréger et l'enregistre.

  Renvoie 1 si un robot auquel s'agréger est trouvé.
  Renvoie 0 sinon
*/
    int8_t i;
		if(!mydata->nb_voisins){
			return 0;
		}
    for (i = mydata->nb_voisins-1; i >= 0; i--) {
        if (mydata->voisins_liste[i].nb_voisins >= mydata->toAggregate.nb_voisins && mydata->voisins_liste[i].id!=IDOBSTACLE){
            mydata->toAggregate = mydata->voisins_liste[i];
        }
    }
    return 1;
}

int main() {
	/*
Initialise callback et lance la main loop
*/
    kilo_init();

    kilo_message_rx = message_rx;
    kilo_message_tx = message_tx;
		SET_CALLBACK(botinfo, botinfo);

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
  p+= sprintf (p, "ID: %d ", kilo_uid);

  // p+= sprintf (p, "move: %d wait:%d turn:%d\n", mydata->move, mydata->wait, mydata->turn);

  p += sprintf (p, "%d neighbors: ", mydata->nb_voisins);
	for(i=0; i<mydata->nb_voisins;i++)
			p+=sprintf(p, "[id : %d, dist : %d, time : %d ]\n",mydata->voisins_liste[i].id,mydata->voisins_liste[i].dist,kilo_ticks-mydata->voisins_liste[i].timestamp);
  p+=sprintf(p,"\nhas mur %d last update mur %d delai : %d\n",hasMur(),mydata->mur_update,mydata->delai);

  return botinfo_buffer;
}



#endif
