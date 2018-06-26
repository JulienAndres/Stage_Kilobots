#ifndef MEDEA
    #define MEDEA

    #define SECONDE 32
    #define MAXVOISIN 30
    #define GENOMEPARAM 10
    #define MAXROBOT 100
    #define TIMEUPDATE 40
    #define IDFOOD 99
    #define PROBA_MUTATION 0.1
    #define IDOBSTACLE 0

    #define isMur(id) ((id==IDOBSTACLE)?1:0 )

void setup();
void genome_alea();
void loop();
int main();
int fitness();
void set_motion(uint8_t motion);
void genome_motion();
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
int arrondi(double n);
uint8_t hasMur(void);

    typedef struct {
      uint8_t dist;
      uint8_t id;
      uint32_t timestamp;
    } Neighbor_t;


    typedef struct{
      uint8_t genome[GENOMEPARAM];
      uint8_t id;
      uint8_t fitness;
    }Genome_t;

    typedef struct
    {
      uint8_t nb_voisins;
      Neighbor_t voisins_liste[MAXVOISIN];

      uint8_t curr_motion;
    	uint32_t last_motion_update;

    	uint8_t new_message;

      uint32_t last_fitness[TIMEUPDATE];

      uint8_t last_update_fitness;
      uint32_t time_update_fitness;

      message_t msg_transmis;
      message_t message;

      uint8_t message_dist;
      uint8_t broadcast;
      uint8_t message_sent;
      uint8_t genome[GENOMEPARAM];

      uint8_t nb_genome;
      Genome_t genome_list[MAXROBOT];
      uint32_t last_genome_update;
      uint8_t genome_setup;
      uint8_t dead;

      //MUR
      uint8_t mur_dist;
      uint8_t last_mur_dist;
      uint32_t mur_update;

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
