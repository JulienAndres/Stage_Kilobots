#ifndef MEDEA
    #define MEDEA

    #define SECONDE 32
    #define MAXVOISIN 30
    #define GENOMEPARAM 8
    #define MAXROBOT 100
    #define TIMEUPDATE 40
    #define IDFOOD 0
    #define PROBA_MUTATION 0

void setup();
void genome_alea();
void loop();
int main();
int fitness();

void do_stats();

int16_t callback_obstacles(double x, double y, double *m1, double *m2);
char *botinfo(void);
json_t *json_state();

    typedef struct {
      uint8_t dist;
      uint16_t id;
      uint32_t timestamp;
      uint32_t nb_voisins;
    } Neighbor_t;


typedef struct{
  uint8_t genome[GENOMEPARAM];
  uint8_t id;
  uint8_t fitness;
  uint32_t parent;//départ du genome
}Genome_t;

    typedef struct
    {

      uint8_t state;
      uint32_t nb_voisins;
      Neighbor_t voisins_liste[MAXVOISIN];

    	uint8_t curr_motion;
    	uint32_t last_motion_update;
      uint32_t start_repelling;
    	uint8_t new_message;

      uint8_t dist;
      uint8_t last_dist_update;
      uint32_t last_fitness[TIMEUPDATE];
      uint32_t fitness;
      uint8_t last_update_fitness;
      uint32_t time_update_fitness;

      Neighbor_t toAggregate;
      message_t msg_transmis;
      message_t message;
      uint8_t message_dist;
      uint8_t broadcast;
      uint8_t message_sent;
      int genome[GENOMEPARAM];
      uint32_t parent;
      uint16_t nb_genome;
      Genome_t genome_list[MAXROBOT];
      uint32_t last_genome_update;
      uint8_t genome_setup;
      uint8_t dead;
      uint32_t last_allowed;

      FILE* fichier;
      uint8_t ecrire;

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
