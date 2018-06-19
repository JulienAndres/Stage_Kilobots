#ifndef EVITE
    #define EVITE

    #define SECONDE 32
    #define MAXVOISIN 30
    #define IDOBSTACLE 0
    #define DISTOBSTACLE 100 //pour une intensité de 24V
    #define RB_SIZE 8

    //setup && loop
    void setup();
    void loop();
    int main();

    //behaviour
    void evitement_obstacle();

    //utilities
    uint8_t hasMur();
    uint8_t isMur();



    typedef struct {
      uint8_t dist;
      uint16_t id;                /*!< the last known dist of the neighbor */
      uint32_t timestamp;     /*!<the tick where the last information was received from this neighbor ) */
      uint32_t nb_voisins;
    } Neighbor_t;

    typedef struct {
        message_t msg;                                  /*!< the content of the message */
        distance_measurement_t dist;         /*!< the measured dist of the message */
    } received_message_t;

    typedef struct
    {
      //COMMUNICATION
      uint8_t broadcast;
      uint8_t new_message;
      uint8_t message_dist;
      message_t msg_transmis;
      message_t message;
      uint8_t cpt_message;

      uint8_t RXHead, RXTail;
      received_message_t RXBuffer[RB_SIZE];
      uint8_t repel;

      uint32_t rotation_timer;
      uint8_t x_old;
      uint8_t x_new;
      uint8_t wall_detected;
      uint32_t wall_detected_timeout;
      //MUR
      uint8_t mur_dist;
      uint8_t last_mur_dist;
      uint32_t mur_update;

      //MOTION LOCK
      uint32_t lock_update;
      uint8_t lock;

      //GESTION VOISINS
      Neighbor_t voisins_liste [MAXVOISIN];
      uint16_t nb_voisins;

      //GESTION MOUVEMENT
      uint32_t last_motion_update;
      uint8_t curr_motion;
      uint8_t state;
      uint32_t delai;

    } USERDATA;

    enum motion {
      STOP,                /*!< The robot is stopped*/
      LEFT,                 /*!< The robot is moving to the left */
      RIGHT,               /*!< The robot is moving to the right */
      STRAIGHT         /*!< The robot is moving straight */
    };

    enum state {
    SEARCHING,             /*!< The robot is searching for an other robot */
    ROTATION,
    ESCAPE,
    INIT
    };


    char *botinfo(void);

#endif
