#include <kilolib.h>
#define DEBUG
#include <debug.h>


//

// Flag to keep track of new messages.
int new_message = 0;
int nb_messages=0;
message_t *m;
int dist=0;


void setup()
{
  nb_messages=0;
  printf("nb message recu%d\n",nb_messages );

}

void loop()
{
    // Blink the LED yellow whenever a message is received.
    if (new_message == 1)
    {
        // Reset the flag so the LED is only blinked once per message.
        printf("message %d \n",m->data[0] );
        printf("distance %d \n",dist );
        new_message = 0;
        nb_messages++;

        set_color(RGB(3, 3, 3));
    }
    set_color(RGB(0, 0, 0));
    // printf("nb message recu%d\n",nb_messages );
}

void message_rx(message_t *message, distance_measurement_t *distance)
{
    // Set the flag on message reception.
    new_message = 1;
    m=message;
    dist=estimate_distance(distance);
}

int main()
{
    kilo_init();
    debug_init();
    // Register the message_rx callback function.
    kilo_message_rx = message_rx;
    kilo_start(setup, loop);

    return 0;
}
