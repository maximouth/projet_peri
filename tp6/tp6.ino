#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

#define MAX_WAIT_FOR_TIMER 3

enum state {EMPTY, FULL} ;

struct mailbox {
  enum state state;
  int val;
  char * mess = (char *)malloc (50 * sizeof (char));
} ;

struct mailbox mb;

char * mess = (char *)malloc (50 * sizeof (char));

int clign = 1;

void T1 (struct mailbox *mb) {
  char lec;
  int i = 0;
  String messlu;

  if (mb->state != EMPTY) return; // attend que la mailbox soit vide
  
  if (Serial.available() > 0) {
    if ( (lec = Serial.read()) == 'P') {
      messlu = Serial.readString();
      mb->mess = strdup(messlu.c_str());
    }
    mb->val = lec;
  }

  mb->state = FULL;
}


void T2 (struct mailbox *mb) {
  if (mb->state != FULL) return; // attend que la mailbox soit pleine
  // usage de mb->val
  if (mb->val == 'A') {
    clign = 1;
  }
  else if (mb->val == 'E') {
    clign = 0;
  }
  else if (mb->val == 'P') {
    mess = mb->mess;
  }
  mb->state = EMPTY;
}



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

void Led(int timer, long period, byte led) {
  // valeur à mettre sur la led
  static byte val = 0;                        
  // sort s'il y a moins d'une période écoulée
  if (!waitFor(timer,period)) return;                     
  // ecriture
  if (clign == 1) {
    //si on a un A
  digitalWrite(led,val);
  // changement d'état
  val = 1 - val;   
  }
  else {
    //si on a un E    
    digitalWrite(led,0);
  }                           
}

void Mess(int timer, long period, const char * mess) {
  // sort s'il y a moins d'une période écoulée
  if (!(waitFor(timer,period))) return; 
  // affichage du message
  Serial.println(mess);
}

int t = 1;

void Aff (int timer, long period) {
  // sort s'il y a moins d'une période écoulée
  if (!(waitFor(timer,period))) return; 

  // affichage du message
  if (t == 1) {
    display.clearDisplay();
    display.invertDisplay(false);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println(mess);
    t = 0;
  }
  else {
     display.clearDisplay();
     display.invertDisplay(true);
     display.setTextSize(1);
     display.setTextColor(WHITE);
     display.setCursor(0,0);
     display.println(mess);
     t = 1;
  }
  display.display();
}



void setup() {
  // initialisation de la direction de la broche
  pinMode(13,OUTPUT);                           

  // initialisation du débit de la liaison série
  Serial.begin(115200);               

  // initialize with the I2C addr 0x3C (for the 128x32)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.display();
  delay(2000);
  display.clearDisplay();

  mess = "message vide";
  mb.state = EMPTY;
}



void loop() {
  // Led est exécutée toutes les 100ms
  Led (0,100000,13);                      
  // Mess est exécutée toutes les secondes
  Mess(1,1000000, mess);     
  Aff (2, 1000000);
  T1(&mb);
  T2(&mb);
}
