#include <kilombo.h>

#include "evite.h"
#include "movement.h"
#include "communication.h"


REGISTER_USERDATA(USERDATA)

void setup() {

  //STATE
  mydata->state = SEARCHING;
  set_color(RGB(1,0,0));

  //COMMUNICATION
  mydata->new_message=0;
  mydata->broadcast = 0;
  //WALL
  mydata->x_old = 0;
  mydata->x_new = 0;
  mydata->wall_detected = 0;
  mydata->rotation_timer = 0;

  //MOVEMENT
  mydata->last_motion_update=kilo_ticks;
  set_random_direction();

  return;
}

void loop(){

  if(kilo_uid==0){
    set_motion(STOP);
    set_color(RGB(0, 0, 1));
    if(!mydata->broadcast){
       printf("lala"); mydata->broadcast = 1;
    }
    return;
  }

	if(kilo_ticks > mydata->wall_detected_timeout) {
		mydata->wall_detected = 0;
    mydata->state = SEARCHING;
  }

  switch(mydata->state){
    case SEARCHING:
    //printf("SEARCHING\n");
      set_color(RGB(0,1,1));
      if(kilo_ticks > mydata->last_motion_update + SECONDE){
        mydata->last_motion_update=kilo_ticks;
        set_random_direction();
        if(mydata->new_message == 1){
          mydata->new_message = 0;
          mydata->state = INIT;
        }
      }
    break;

    case ROTATION:
    //printf("ROTATION\n");
    set_color(RGB(1,0,0));
    if(kilo_ticks > mydata->rotation_timer){
      mydata->rotation_timer = kilo_ticks + 7*SECONDE;
      if(mydata->curr_motion == LEFT){
        set_motion(RIGHT);
      } else {
        set_motion(LEFT);
      }
      RB_init();
      mydata->x_old = 0;
      mydata->x_new = 0;

      set_color(RGB(1,0,1));
    }
    if(mydata->new_message == 1){
      mydata->new_message = 0;
      if(mydata->x_old < mydata->x_new){
        if(mydata->curr_motion != STRAIGHT){
          set_motion(STRAIGHT);
          //set_motors(kilo_straight_left, kilo_straight_right);
        }
        printf("motion avant : %d\n", mydata->curr_motion);
        set_color(RGB(1,1,1));
        mydata->state = ESCAPE;
        printf("TO ESCAPE\n");
      }
    }
    break;

    case ESCAPE:
      if(mydata->new_message == 1){
        mydata->new_message = 0;
        printf("il rentre là dedans");
        printf("motion : %d\n", mydata->curr_motion);
        if(mydata->x_old > mydata->x_new){
          mydata->state = INIT;
        }
      }
      break;

    case INIT:
    //printf("INIT\n");
      set_random_turning_direction();
      RB_init();
      mydata->rotation_timer = kilo_ticks + 7*SECONDE;
      set_color(RGB(1,1,1));
      mydata->state = ROTATION;
      break;

  }
    //printf("NEW MESSAGE : %d\n", mydata->new_message);
}

uint8_t isMur(int ID){
  return ID==IDOBSTACLE;
}

int main() {
	/*
Initialise callback et lance la main loop
*/
    kilo_init();
    RB_init();
    kilo_message_rx = rxbuffer_push;
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
  p += sprintf(p, "State : %d\n",mydata->state);
  return botinfo_buffer;
}



#endif
