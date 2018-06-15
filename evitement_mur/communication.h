#ifndef COMMUNICATION
  #define COMMUNICATION

void update_voisins();
void update_mur_from_message();
void update_from_message();
void message_rx (message_t *message, distance_measurement_t *distance);
void setup_message();
message_t *message_tx();

//void emission();


#endif
