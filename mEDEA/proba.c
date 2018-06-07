#include <kilombo.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "proba.h"
#include "mEDEA.h"
#include "communication.h"

extern USERDATA * mydata;

double probaBroadcast(uint8_t dist){//TODO proba en fonction de la distance
  return 1;
}

void canBroadcast(){
  if(kilo_ticks<mydata->last_allowed+2*SECONDE){
    return;
    // return mydata->msg_transmis.data[8];
  }
  mydata->last_allowed=kilo_ticks;
  int i;
  for(i=0;i<mydata->nb_voisins;i++){
    if (isfood(mydata->voisins_liste[i].id)){
      double proba=probaBroadcast(mydata->voisins_liste[i].dist);
      int rd=rand();
      // printf("%d\n", RAND_MAX*(1-proba));
      if (rd> RAND_MAX*(1-proba)){
        share_genome_allowed();
        return;
        // return 1;
      }
      else{
        share_genome_forbiden();
        return;
        // return 0;
      }
    }
  }
  share_genome_forbiden();
  return;
  // return 0;
}

int arrondi(double n){
	return n+0.5;
}
