#include "kilombo.h"

#include<stdio.h>
#include <stdlib.h>
#include "run_tumble.h"
#include "movement.h"
#include "communication.h"


REGISTER_USERDATA(USERDATA)


/*
Fitness : 0 si pres d'un mur
          1 si près d'un mur et du bloc
          2 si près du bloc.

*/

void setup()
{
  mydata->broadcast=0;
  mydata->curr_motion=-1;
  mydata->state=GO;
  mydata->turntimer=rand_hard()%4;
  mydata->proba=rand()%1000;//pour simuateur
  // mydata->proba=275;
  // mydata->proba=RAND_MAX-2*rand_hard();
  printf("%d\n",mydata->proba );

  mydata->nb_genome=0;
  uint8_t i;
  for(i=0;i<9;i++){
    mydata->msg_transmis.data[i]=0;
  }
  setup_message();
  mydata->dead=0;
  mydata->broadcast=1;
  mydata->new_message=0;
  mydata->last_genome_update=kilo_ticks;
  mydata->proba_update=kilo_ticks;

  // #ifdef SIMULATOR
  // char nu[7];
  // snprintf(nu,7,"%d",kilo_uid);
  // char name[50]="stats_robot/";
  // strcat(name,nu);
  //  mydata->fichier=fopen(name,"w");
  //  mydata->ecrire=1;
  //  #endif


}

void genome_alea()
{
  if(!mydata->nb_genome){
    mydata->dead=1;
    mydata->last_genome_update=kilo_ticks;
    return;
  }
  uint8_t picked=rand_hard()%mydata->nb_genome;
  mydata->proba=mydata->genome_list[picked].proba;
  mydata->nb_genome=0;
  mydata->last_genome_update=kilo_ticks;
  mutation();
  if (mydata->proba > RAND_MAX) mydata->proba=RAND_MAX;
  setup_message();

}

void mutation(){
  if (((float)rand_hard())/256 < PROBA_MUTATION){
    int signe=rand_hard()%2;
    int value=rand_hard()%100; //Variation de la mutation
    if (!signe){
      mydata->proba-=value;
    }else{
      mydata->proba+=value;
    }
  }
}




void loop()
{

//   #ifdef SIMULATOR
//   if (kilo_ticks%(60*SECONDE)==0 && mydata->ecrire==1){
// 		// fprintf(mydata->fichier, "%d\n",mydata->dead );
// 		// if(mydata->dead){
// 		// 	fprintf(mydata->fichier, "%d\n",-1 );
// 		// }
// 		// else{
// 		// 	fprintf(mydata->fichier, "%d\n",mydata->parent );
// 		// }
// 		if(mydata->dead){
// 			fprintf(mydata->fichier, "%d\n",-1 );
// 		}else{
// 			// printf("%d\n",kilo_uid );
// 			// printf("%d%d%d%d%d%d%d%d\n",mydata->genome[0],mydata->genome[1],mydata->genome[2],mydata->genome[3],mydata->genome[4],mydata->genome[5],mydata->genome[6],mydata->genome[7]);
// 			fprintf(mydata->fichier, );
// 			// printf("%d\n",kilo_uid );
// }
//
// 		mydata->ecrire=0;
// 		// printf("%d\n",kilo_uid );
// 	}
// if (kilo_ticks%SECONDE==1){
// 	mydata->ecrire=1;
// }
//   #endif




  if (mydata->dead){
    set_color(0);
    set_motion(STOP);
    return;
  }
  if(kilo_ticks >= mydata->last_genome_update+60*SECONDE){
    set_color(RGB(3,3,3));
    genome_alea();
  }


int rnd=rand()%32767;
// printf("proba %d rnd %d\n",mydata->proba,rnd );
if(mydata->new_message) update_from_message();



if(kilo_ticks>mydata->proba_update+SECONDE/4)
{
  mydata->proba_update=kilo_ticks;
  switch (mydata->state) {
    case GO:
      set_color(RGB(3,0,0));
      set_motion(STRAIGHT);
      if (rnd<mydata->proba){
        mydata->state=TURN;
        set_random_turning_direction();
        mydata->last_motion_update=kilo_ticks;
        mydata->turntimer=rand_hard()%4;

        #ifdef SIMULATOR
        mydata->turntimer=rand_hard()%15;
        #endif

      }else{
      }
      break;
    case TURN:
      set_color(RGB(0,3,0));
      if(kilo_ticks > mydata->last_motion_update + mydata->turntimer*SECONDE){
        set_motion(STRAIGHT);
        mydata->state=GO;
      }
      break;
  }
}



}

int main()
{
  kilo_init();
  kilo_message_tx=message_tx;
  kilo_message_rx = message_rx;
  SET_CALLBACK(botinfo, botinfo);
  SET_CALLBACK(obstacles, callback_obstacles);

  kilo_start(setup,loop);
  return 0;
}



#ifdef SIMULATOR


#define MUR 500

int16_t callback_obstacles(double x, double y, double *m1, double *m2){

	//CERCLE
	// if(x*x + y*y >= MUR*MUR){
	// 		*m1 = (x<0)? 1:-1;
	// 		*m2 =  (y<0)? 1:-1;
	// 		return 1;
	// }
	// else{
	// 		return 0;
	// }

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


static char botinfo_buffer[10000000];
// provide a text string for the status bar, about this bot
char *botinfo(void)
{
  int i;
  char *p = botinfo_buffer;
  p+= sprintf (p, "ID: %d state %d proba : %d is dead %d\n", kilo_uid,mydata->state,mydata->proba,mydata->dead);

  // p+= sprintf (p, "move: %d wait:%d turn:%d\n", mydata->move, mydata->wait, mydata->turn);

  p += sprintf (p, "%d genome: ", mydata->nb_genome);
	for(i=0; i<mydata->nb_genome;i++)
			p+=sprintf(p, "[id : %d proba : %d]\n",mydata->genome_list[i].id,mydata->genome_list[i].proba);
  return botinfo_buffer;
}

#endif
