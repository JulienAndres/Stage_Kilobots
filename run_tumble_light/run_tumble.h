#ifndef RUN_TUMBLE
  #define RUN_TUMBLE

  #define SECONDE 32
  #define PROBA 0.999
  #define LAMBDA 3

  void setup(void);
  void loop(void);
  message_t* message_tx(void);
  int main(void);

  typedef struct
  {

    //GESTION MOUVEMENT
    uint32_t last_motion_update;
    uint8_t curr_motion;
    uint32_t proba;
    uint8_t turntimer;

    uint8_t state;

    uint8_t broadcast;

    float lambda;

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
