#ifndef AGGREGATION
    #define AGGREGATION

    #define SECONDE 32
    #define MAXVOISIN 30
    #define IDOBSTACLE 0
    #define DISTOBSTACLE 70 //pour une intensité de 24V
    #define DIST_TO_AGGREGATE 50
    #define CLUSTER_SIZE 6

    //setup && loop
    void setup(void);
    void loop(void);
    int main(void);

    //behaviour
    void evitement_obstacle(void);
    void repelling(void);
    void sleeping(void);
    void converging(void);

    //utilities
    uint8_t hasMur(void);
    uint8_t isMur(int);
    uint8_t is_too_close(void);
    uint8_t is_too_close1(void);
    uint8_t hasBestNeighbor(void);

    //SIMULATOR
    #ifdef SIMULATOR
    char *botinfo(void);
    int16_t callback_obstacles(double x, double y, double *m1, double *m2);
    #endif

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

      //STATE OF ROBOT
      uint8_t state;

      //GESTION AGGREGATION
      Neighbor_t toAggregate;
      uint8_t last_dist_update;

      //GESTION REPELLING
      uint32_t start_repelling;

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



#endif
