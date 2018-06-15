#include <kilolib.h>
#define DEBUG
#include <debug.h>

message_t message;
// Flag to keep track of message transmission.
int message_sent = 0;
uint32_t nb_message;
int stop=0;
void setup()
{
    // Initialize message:
    // The type is always NORMAL.
    message.type = NORMAL;
    // Some dummy data as an example.
    message.data[0] = 0;
    // It's important that the CRC is computed after the data has been set;
    // otherwise it would be wrong and the message would be dropped by the
    // receiver.
    message.crc = message_crc(&message);
    stop=0;
    nb_message=0;
}

void loop()
{
  if(nb_message>=120){
    printf("nb message envoye %d\n",nb_message );
    set_color(RGB(3,3,3));
    stop=1;
    return;
  }
    // Blink the LED magenta whenever a message is sent.
    if (message_sent == 1)
    {
        // Reset the flag so the LED is only blinked once per message.
        message_sent = 0;
        nb_message++;
        set_color(RGB(3, 0, 0));
    }
    set_color(RGB(0, 0, 0));
    printf("nb message envoye %d\n",nb_message );

}

message_t *message_tx()
{
  if(!stop)
    return &message;
  else
  return 0;
}

void message_tx_success()
{
    // Set the flag on message transmission.
    message_sent = 1;


}

int main()
{
    kilo_init();
    debug_init();

    // Register the message_tx callback function.
    kilo_message_tx = message_tx;
    // Register the message_tx_success callback function.
    kilo_message_tx_success = message_tx_success;
    kilo_start(setup, loop);

    return 0;
}
