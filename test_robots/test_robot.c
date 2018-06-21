#include "kilolib.h"
#define DEBUG

#include "debug.h"
#include <stdio.h>


float a;

void setup(){
	
	a=1.345678;
}

void loop(){


printf("uid : %d m_d : %d m_g : %d\n",kilo_uid,kilo_straight_right,kilo_straight_left);
printf("turn_right : %d turn_left : %d\n",kilo_turn_right,kilo_turn_left);

a+=0.01;
	printf("%.7f\n",a );

}




int main(){
kilo_init();
debug_init();
kilo_start(setup,loop);


return 0;
}
