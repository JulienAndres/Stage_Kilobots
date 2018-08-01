#ifndef COMMUNICATION
  #define COMMUNICATION

int fitness(void);
int fitness2(void);
void update_from_message();
void message_rx (message_t *message, distance_measurement_t *distance);
void setup_message();
message_t *message_tx();

//void emission();


#endif
