#ifndef EVITE
    #define EVITE

    #define SECONDE 32
    #define MAXVOISIN 30
    #define IDOBSTACLE 0
    #define DISTOBSTACLE 70 //pour une intensité de 24V

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

    typedef struct
    {
      //COMMUNICATION
      uint8_t broadcast;
      uint8_t new_message;
      uint8_t message_dist;
      message_t msg_transmis;
      message_t message;

      //MUR
      uint8_t mur_dist;
      uint8_t last_mur_dist;
      uint32_t mur_update;

      //GESTION VOISINS
      Neighbor_t voisins_liste [MAXVOISIN];
      uint16_t nb_voisins;

      //GESTION MOUVEMENT
      uint32_t last_motion_update;
      uint8_t curr_motion;
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
    CONVERGING,          /*!< The robot is moving to a neighbor */
    SLEEPING,               /*!< The robot is sleeping */
    REPELLING
    };


    char *botinfo(void);

#endif
