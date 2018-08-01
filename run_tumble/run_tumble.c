#include "kilombo.h"
#include <stdlib.h>
#include <math.h>
#include "run_tumble.h"
#include "movement.h"


REGISTER_USERDATA(USERDATA)


void setup()
{
  mydata->broadcast=0;
  mydata->curr_motion=-1;
  mydata->state=GO;
  mydata->turntimer=rand_hard()%4;

  mydata->proba=(rand()%32767);
  mydata->proba=PROBA;
  printf("%d\n",mydata->proba );

}

void loop()
{
int rnd=(rand()%32767);
printf("proba %d rnd %d\n",mydata->proba,rnd );
switch (mydata->state) {
  case GO:
    set_color(RGB(3,0,0));
    set_motion(STRAIGHT);
    if (rnd>mydata->proba){
      mydata->state=TURN;
      set_random_turning_direction();
      mydata->last_motion_update=kilo_ticks;
      mydata->turntimer=rand_hard()%4;
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

message_t* message_tx()
{
  if(!mydata->broadcast) return 0;
  return 0;
}

int main()
{
  kilo_init();
  kilo_message_tx=message_tx;

  kilo_start(setup,loop);
  return 0;
}
