#include <kilombo.h>
#include <stdio.h>
#include <math.h>
#include "proba.h"
#include "aggregation.h"
#include "movement.h"

extern USERDATA * mydata;


void makeLeaveDecision(){
  /*
    Calcule une probabilite de partir et met à jour l'état du robot si besoin.
  */

    // uint32_t p=clusters_proba();
    // uint32_t p=uniform_proba();
    uint32_t p=clusters_proba3(nbvoisin_bestvoisin());
    if (rand() > p){
        mydata->state = REPELLING;
        mydata->curr_motion = rand_hard()%2 + 1;
        mydata->start_repelling = kilo_ticks;
    }
}

int uniform_proba(){
  /*
    Proba constante de quitter le cluster.
      Calculé selon PROBA_LEAVE dans .h
  */
  return RAND_MAX*(1-PROBA_LEAVE );
}

int clusters_proba(){
  /*
    Proba en fonction du nmbre de voisin du robot
    + le nombre de voisin est petit plus la proba de quitter est grande
  */
  float p=((float)mydata->nb_voisins/(float)CLUSTER_SIZE);
  p=(p<1)? p:1;
  return p*RAND_MAX;
}


int clusters_proba3(int nb){
  /*
    Proba en fonction du nombre de voisin donné en parametre.
    nb=nombre de voisins max de ses voisins.

    return proba en fonction du max entre son propre nmbre de voisins et parametre.
  */
  nb=(nb>mydata->nb_voisins)? nb:mydata->nb_voisins;
  float p=((float)nb/(float)CLUSTER_SIZE);
  p=(p<1)? p:1;
  return p*RAND_MAX;
}

int nbvoisin_bestvoisin(){
  /*
    return nombre de voisins max de tous les voisins d'un robot.
  */
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
