#ifndef EVITE
    #define EVITE

    #define SECONDE 32
    #define MAXVOISIN 30
    #define IDOBSTACLE 0
    #define DISTOBSTACLE 100 //pour une intensité de 24V
    #define RB_SIZE 4

    //setup && loop
    void setup();
    void loop();
    int main();

    //behaviour

    //utilities
    uint8_t isMur();

    typedef struct {
        message_t msg;                                  /*!< the content of the message */
        distance_measurement_t dist;         /*!< the measured dist of the message */
    } received_message_t;

    typedef struct
    {
      //COMMUNICATION
      uint8_t new_message;
      message_t msg_transmis;

      uint8_t RXHead, RXTail;
      received_message_t RXBuffer[RB_SIZE];
      uint8_t repel;

      uint32_t rotation_timer;
      uint16_t x_old;
      uint16_t x_new;
      uint8_t wall_detected;
      uint32_t wall_detected_timeout;

      uint8_t broadcast;

      //GESTION MOUVEMENT
      uint32_t last_motion_update;
      uint8_t curr_motion;

      uint8_t state;

    } USERDATA;

    enum motion {
      STOP,                /*!< The robot is stopped*/
      LEFT,                 /*!< The robot is moving to the left */
      RIGHT,               /*!< The robot is moving to the right */
      STRAIGHT        /*!< The robot is moving straight */
    };

    enum state {
    SEARCHING,             /*!< The robot is searching for an other robot */
    ROTATION,
    ESCAPE,
    INIT
    };


    char *botinfo(void);

#endif
