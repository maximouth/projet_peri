#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "RF24.h"

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define PHR 15

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

RF24 radio (9,10);

byte addresses[][6] = {"01234","23456","2Node","3Node","4Node","5Node"};

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

#define MAX_WAIT_FOR_TIMER 3


int val;


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

  val = analogRead (PHR);
  
  // envoie de la valeur
  radio.stopListening();
  radio.write (&val, 6);

  // affichage du message
     display.clearDisplay();
    display.invertDisplay(false);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println(val);

    display.display();
}



void setup() {
  
  radio.begin();
  radio.setChannel(69);
  radio.setRetries(15,15);
  radio.setPALevel(RF24_PA_LOW);
  //radio.openReadingPipe(1,addresses[0]);
  radio.openWritingPipe(addresses[0]);
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

}



void loop() {
  // Led est exécutée toutes les 100ms
  //Led (0,100000,13);                      
  Aff (2, 1000000);
}
