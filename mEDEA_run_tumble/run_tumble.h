#ifndef RUN_TUMBLE
  #define RUN_TUMBLE

  #define SECONDE 32
  #define PROBA_MUTATION 0.1
  #define MAXROBOT 32
  #define NBFITNESS 40
  #define DIST_OBSTACLE 80

 #define PROBA (rand()%32767)


  void setup(void);
  void genome_alea(void);
  void mutation(void);
  void loop(void);
  message_t* message_tx(void);
  int main(void);
  int fitness();

  char *botinfo(void);
  int16_t callback_obstacles();

  typedef struct
  {
    uint8_t id;
    uint32_t proba;
    uint16_t fitness;
  }Genome_t;


  typedef struct
  {
    uint8_t turntimer; //temps de rotation
    uint8_t nb_genome;//nb de genome recu
    uint8_t new_message;//flag is message recu
    uint32_t last_genome_update;//

    message_t msg_transmis;//message transmis.(mis à jour à chaque changement de genome.
    message_t message;//message recu par le robot.

    uint8_t dead;//flag si il est mort

    Genome_t genome_list[MAXROBOT];
;
    //GESTION MOUVEMENT
    uint32_t last_motion_update;
    uint8_t curr_motion;

    uint8_t state;

    uint8_t broadcast;
    uint32_t proba;

    uint32_t proba_update;

    uint16_t cpt_msg;

    #ifdef SIMULATOR
    // FILE* fichier;
    // uint8_t ecrire;
    //

    FILE* fichier;
    uint8_t ecrire;

    FILE* fichier2;
    uint8_t ecrire2;
    #endif

  } USERDATA;


  enum motion {
    STOP,                /*!< The robot is stopped*/
    LEFT,                 /*!< The robot is moving to the left */
    RIGHT,               /*!< The robot is moving to the right */
    STRAIGHT         /*!< The robot is moving straight */
  };

  enum state{
    GO,
    TURN
  };

  #endif
