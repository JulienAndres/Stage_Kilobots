#ifndef COMMUNICATION
  #define COMMUNICATION

void update_voisins();
void update_mur_from_message();
void update_from_message();
void message_rx (message_t *message, distance_measurement_t *distance);


void rxbuffer_push(message_t *msg, distance_measurement_t *dist);

void setup_message();
message_t *message_tx();

#define RB_init() { \
    mydata->RXHead = 0; \
    mydata->RXTail = 0;\
}

#define RB_empty() (mydata->RXHead == mydata->RXTail)

#define RB_full()  ((mydata->RXHead+1)%RB_SIZE == mydata->RXTail)

#define RB_front() mydata->RXBuffer[mydata->RXHead]

#define RB_back() mydata->RXBuffer[mydata->RXTail]

#define RB_popfront() mydata->RXHead = (mydata->RXHead+1)%RB_SIZE;

#define RB_pushback() {\
    mydata->RXTail = (mydata->RXTail+1)%RB_SIZE;\
    if (RB_empty())\
      { mydata->RXHead = (mydata->RXHead+1)%RB_SIZE;  \
     }        \
}

#endif
