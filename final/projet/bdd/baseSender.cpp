#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <RF24/RF24.h>
#include <ctime>
#include <cstdio>


using namespace std;

typedef uint8_t byte;

struct data {
  int val;
  unsigned long millis;
  bool reset;
};
typedef struct data data;


/***********************
 * Variables globales
 ***********************/

RF24 radio(15,8);
byte addresses[][6] = {"0Node","1Node","2Node","3Node","4Node","5Node"};
data donnee;


void setup() {
  radio.begin();
  radio.setChannel(69);
  radio.setRetries(15,15);
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(addresses[0]);
  radio.printDetails();
  radio.startListening();
}

void loop() {
  while(donnee.val == 0) {
    cout << "Val = ";
    cin >> donnee.val;
    cout << "Millis(9) = ";
    cin >> donnee.millis;
    // cout << "Reset(0|1) = ";
    // cin >> donnee.reset;
    donnee.reset = 0;
  }
  radio.stopListening();
  radio.write (&donnee, sizeof(data));
  donnee.val = 0;
}

int main(int argc, char** argv){
  setup();
  while (1) loop();
  return 0;
}
