#include <kilombo.h>
#include "evite.h"
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
				//set_motors(kilo_straight_left, kilo_straight_right);
				set_motors(kilo_turn_left, kilo_turn_right);
				break;
			case STOP:
				default:
				set_motors(0, 0);
			break;
		}
		mydata->curr_motion = motion;
	}
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
	uint8_t random_number = rand_hard()%2;
	if(random_number == 0){
		set_motion(LEFT);
	} else {
		set_motion(RIGHT);
	}
}
