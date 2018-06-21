#include "kilombo.h"
#include "math.h"
#include <stdlib.h>
#include "run_tumble.h"
#include "movement.h"

REGISTER_USERDATA(USERDATA)


void setup()
{
  mydata->broadcast=0;
  mydata->curr_motion=STRAIGHT;

}

void loop()
{
double rnd=rand();
rnd/=RAND_MAX;
printf("%f\n",rnd );
switch (mydata->curr_motion) {
  case STRAIGHT:
    set_motion(STRAIGHT);
    if (rnd>PROBA){
      mydata->state=TURNING;
      mydata->last_motion_update=kilo_ticks;
    }
  case LEFT:
  case RIGHT:
    if(kilo_ticks > mydata->last_motion_update + 10*SECONDE){
      set_motion(STRAIGHT);
    }
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
