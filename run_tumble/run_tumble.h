#ifndef RUN_TUMBLE
  #define RUN_TUMBLE

  #define SECONDE 32
  #define PROBA 0.99

  void setup(void);
  void loop(void);
  message_t* message_tx(void);
  int main(void);

  typedef struct
  {

    //GESTION MOUVEMENT
    uint32_t last_motion_update;
    uint8_t curr_motion;

    uint8_t broadcast;

  } USERDATA;


  enum motion {
    STOP,                /*!< The robot is stopped*/
    LEFT,                 /*!< The robot is moving to the left */
    RIGHT,               /*!< The robot is moving to the right */
    STRAIGHT         /*!< The robot is moving straight */
  };

  #endif
