#include <kilombo.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "proba.h"
#include "aggregation.h"

extern USERDATA * mydata;

void makeJoinDecision(){
    if (rand() > RAND_MAX*(1-PROBA_JOIN)){
      printf("makeJoinDecision\n" );
        mydata->state = CONVERGING;
        mydata->curr_motion =  rand_hard()%2 + 1;
    }

}

void makeLeaveDecision(){
    // uint32_t p=clusters_proba2();
    // uint32_t p=clusters_proba();
    // uint32_t p=uniform_proba();
    uint32_t p=clusters_proba3(nbvoisin_bestvoisin());

    if (rand() > p){
      // printf("makeLeaveDecision %d\n" ,kilo_uid);
        mydata->state = REPELLING;
        mydata->curr_motion = rand_hard()%2 + 1;
        mydata->start_repelling = kilo_ticks;
    }
}

int uniform_proba(){
  return RAND_MAX*(1-PROBA_LEAVE );
}
int clusters_proba(){
  float p=((float)mydata->nb_voisins/(float)CLUSTER_SIZE);
  p=(p<1)? p:1;
  return p*RAND_MAX;
}

int clusters_proba2(){
  if (mydata->nb_voisins>CLUSTER_SIZE){
    return RAND_MAX;
  }else{
    return uniform_proba();
  }
}

int clusters_proba3(int nb){
  nb=(nb>mydata->nb_voisins)? nb:mydata->nb_voisins;
  float p=((float)nb/(float)CLUSTER_SIZE);
  p=(p<1)? p:1;
  return p*RAND_MAX;

}

int nbvoisin_bestvoisin(){
  if (!mydata->nb_voisins) return 0;
  int i=0;
  int tmp=mydata->voisins_liste[0].nb_voisins;
  for(i=0;i<mydata->nb_voisins;i++){
    if (mydata->voisins_liste[i].nb_voisins > tmp){
      tmp=mydata->voisins_liste[i].nb_voisins;
    }
  }
  return tmp;
}
