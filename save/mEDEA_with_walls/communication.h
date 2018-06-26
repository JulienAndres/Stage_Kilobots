#ifndef COMMUNICATION
  #define COMMUNICATION

void update_voisins();
int is_feed();
void emission();
void update_only_voisins_from_message();
void update_from_message();
void message_rx (message_t *message, distance_measurement_t *distance);
void setup_message();
message_t *message_tx();
void message_tx_success();
void setup_message_fitness();
void share_genome_allowed();
void share_genome_forbiden();
void update_fitness();

#endif
