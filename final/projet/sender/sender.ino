#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "RF24.h"
#include "Time.h"

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define PHR 15

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

RF24 radio (9,10);

byte addresses[][6] = {"0Node","1Node","2Node","3Node","4Node","5Node"};

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

#define MAX_WAIT_FOR_TIMER 3

struct data {
  unsigned long millis;
  int val;
  boolean reset;
};
typedef struct data data;

data donnee;

unsigned int waitFor(int timer, unsigned long period){
  static unsigned long waitForTimer[MAX_WAIT_FOR_TIMER];
  // numéro de la période modulo 2^32 
  unsigned long newTime = micros() / period;             
  // delta entre la période courante et celle enregistrée
  int delta = newTime - waitForTimer[timer];
  // en cas de dépassement du nombre de périodes possibles sur 2^32 
  if ( delta < 0 ) delta += 1 + (0xFFFFFFFF / period);   
  // enregistrement du nouveau numéro de période
  if ( delta ) waitForTimer[timer] = newTime;
  return delta;
}

// void Led(int timer, long period, byte led) {
//   // valeur à mettre sur la led
//   static byte val = 0;                        
//   // sort s'il y a moins d'une période écoulée
//   if (!waitFor(timer,period)) return;                     
//   // ecriture
//     //si on a un A
//   digitalWrite(led,val);
//   // changement d'état
//   val = 1 - val;   
// }

void Aff (int timer, long period) {
  // sort s'il y a moins d'une période écoulée
  if (!(waitFor(timer,period))) return; 

  /*
   *Initialisation de la struct data
   *Reset si mCourant OV donc retour a 0 ou < 1024ms(large)
   *Signal a RPI de continuer a compter sans repartir a 0
   *-> A faire dans recever
   *1000 pour correspondre a 1 sec
   */
  donnee.val = analogRead(PHR);
  donnee.millis = millis();
  if(donnee.millis < 1024) {
    donnee.reset = true;
  }

  // Envoi de la valeur
  radio.stopListening();
  radio.write (&donnee, sizeof (data));

  // Affichage de la struct sur l'ecran
  display.clearDisplay();
  display.invertDisplay(false);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,8*0);
  display.println(donnee.val);
  display.setCursor(0,8*1);
  display.println(donnee.millis);
  display.setCursor(0,8*2);
  display.println(donnee.reset);
  display.display();

  //Reset du reset
  if (donnee.reset == true)
    donnee.reset = false;
}



void setup() {
  
  radio.begin();
  radio.setChannel(69);
  radio.setRetries(15,15);
  radio.setPALevel(RF24_PA_LOW);
  //radio.openReadingPipe(1,addresses[0]);
  radio.openWritingPipe(addresses[0]);
    radio.setPayloadSize(32);
  radio.printDetails();
  radio.startListening();

  // initialisation de la direction de la broche
  pinMode(13,OUTPUT);                           

  // photo resistance
  pinMode(PHR,INPUT);                           

  // initialize with the I2C addr 0x3C (for the 128x32)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.display();
  delay(2000);
  display.clearDisplay();

  //Initialisation des temps
  donnee.reset = false;
}



void loop() {
  // Led est exécutée toutes les 100ms
  //Led (0,100000,13);                      
  Aff (2, 10000000);
}
