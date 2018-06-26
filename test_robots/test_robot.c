// #include "kilolib.h"
// #define DEBUG

// #include "debug.h"

#include "kilombo.h"
#include <stdio.h>
#include <stdlib.h>

//RAND_MAX=32767
float a;
typedef struct{
	uint8_t a;
} USERDATA;


REGISTER_USERDATA(USERDATA)


void setup(){

	a=1.345678;
	spinup_motors();
}

void loop(){

set_motors(kilo_turn_left,kilo_turn_right);
printf("uid : %d m_d : %d m_g : %d\n",kilo_uid,kilo_straight_right,kilo_straight_left);
printf("turn_right : %d turn_left : %d\n",kilo_turn_right,kilo_turn_left);
uint16_t light=get_ambientlight();

printf("light : %d\n",light );
printf("lightt %d\n",light/100);
printf("lighttt %d\n",light%100 );


a+=0.01;
printf("%.7f\n",a );
printf("%d\n",RAND_MAX );
}




int main(){
kilo_init();
// debug_init();
kilo_start(setup,loop);


return 0;
}
