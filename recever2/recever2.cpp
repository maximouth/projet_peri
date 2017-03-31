#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <RF24/RF24.h>
#include "libgpio.h"


#define GPIO_LED0   4
#define GPIO_LED1   17


int got_time = 0, change = 5;

uint32_t volatile *gpio_base = 0;

typedef uint8_t byte;

using namespace std;

RF24 radio(15,8);

byte addresses[][6] = {"0Node","1Node","2Node","3Node","4Node","5Node"};

void setup() {
  radio.begin();

  radio.setChannel(69);
  radio.setRetries(15,15);
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1, addresses[0]);
  //  radio.openWritingPipe(addresses[0]);
  radio.printDetails();
  radio.startListening();
}

void loop() {

  if( radio.available()){
     radio.read( &got_time, sizeof(int) );             // Get the payload
   }

  if(got_time != change) {
    cout << "Photoresistance : " got_time << endl;
  }

  change = got_time;
    
}

int main(int argc, char** argv){
  int result = setup_gpio_mmap ( &gpio_base );
  if(result < 0) {
    cout << "Erreur mapping !" << endl;
  }

  GPIO_CONF_AS_OUTPUT (gpio_base, GPIO_LED0);

  GPIO_CONF_AS_OUTPUT (gpio_base, GPIO_LED1);

  GPIO_CLR ( gpio_base, GPIO_LED0 );

  GPIO_CLR ( gpio_base, GPIO_LED1 );

  setup();
  while (1) loop();
  return 0;
}
