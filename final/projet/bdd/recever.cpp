#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <RF24/RF24.h>
#include <ctime>
#include <cstdio>
#include "sqlite3.h"

/**************************
 * Calcul de temps
 **************************/
#define NB_MS_DAY  86400000
#define NB_MS_HOUR 3600000
#define NB_MS_MIN  60000
#define NB_MS_SEC  1000
#define div(a, b) (a / b)
#define mod(a, b) (a % b)
#define nbDay(ms) (div(ms, NB_MS_DAY))
#define nbHour(ms) (div(mod(ms, NB_MS_DAY), NB_MS_HOUR))
#define nbMin(ms) (div(mod(mod(ms, NB_MS_DAY), NB_MS_HOUR), NB_MS_MIN))
#define nbSec(ms) (div(mod(mod(mod(ms, NB_MS_DAY), NB_MS_HOUR), NB_MS_MIN), NB_MS_SEC))


using namespace std;

typedef uint8_t byte;

struct data {
  unsigned long millis;
  int val;
  bool reset;
};
typedef struct data data;


/****************************
 * Declaration de fonctions
 ****************************/

struct tm *ulTotm(time_t *ptm, unsigned long millis);
void afficher(unsigned long t);
void setup();
void loop();
static int callback(void *NotUsed, int argc, char **argv, char **azColName);
sqlite3 *bdd_open(const char *path);
int bdd_request(sqlite3 *bdd, const char *sql);
int bdd_init(sqlite3 *bdd);

/***********************
 * Variables globales
 ***********************/

RF24 radio(15,8);
byte addresses[][6] = {"0Node","1Node","2Node","3Node","4Node","5Node"};
static data donnee = {0};
sqlite3 *bdd;
stringstream sql;
time_t temps, tmp;
struct tm *ptm;
tm tm1;
char buffer[80];
string str;

/****************************
 * Fonctions utiles
 ****************************/

struct tm *ulTotm(time_t *ptm, unsigned long millis) {
  int jour, heure, min, sec;
  struct tm *ptemp;

  jour = nbDay(millis);
  heure = nbHour(millis);
  min = nbMin(millis);
  sec = nbSec(millis);

  ptemp = localtime(ptm);
  ptemp->tm_sec += sec;
  ptemp->tm_min += min;
  ptemp->tm_hour += heure;
  ptemp->tm_mday += jour;
  ptemp->tm_yday = jour;

  return ptemp;
}

void afficher(unsigned long t) {
  string str;

  strftime(buffer, 80, "%F %T", localtime(&temps));
  str = buffer;
  cout << "Temps : " << str << endl;
  ptm = ulTotm(&temps, t);
  tmp = mktime(ptm);
  strftime(buffer, 80, "%F %T", localtime(&tmp));
  str = buffer;
  cout << "Temps : " << str << endl;
}


/**********************
 * Fonctions microµ
 ***********************/

void setup() {
  /*
   * nrf24l01+ initialisation
   */
  radio.begin();
  radio.setChannel(69);
  radio.setRetries(15,15);
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1, addresses[0]);
  radio.setPayloadSize(32);
  radio.printDetails();
  radio.startListening();
  /*
   * time initialisation
   * temps = date + heure locale en sec
   */
  time(&temps);
  ptm = &tm1;
}

void loop() {
  if( radio.available()) {
    radio.read( &donnee, sizeof(data) );
    /*
     * Si Reset, on reprend du temps actuel
     */
    if(donnee.reset)
      time(&temps);
   
    ptm = ulTotm(&temps, donnee.millis);
    tmp = mktime(ptm);
    strftime(buffer, 80, "%F %T", localtime(&tmp));
    str = buffer;
    str.insert(0, 1, '\'');
    str.insert(str.length(), 1, '\'');

    cout << endl;
    cout << "(" << donnee.val << ", " << str << ")";

    /*
     * Requete SQL
     */
    sql.str("");
    cout << sql.str() << endl;
    sql << "INSERT INTO Photoresistance (valeur, date)\n"
  	<< "VALUES (" << donnee.val << ", " << str << ");\n";
    bdd_request(bdd, sql.str().c_str());
  }
}



/****************************
 * Fonctions BDD
 ****************************/

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
  int i;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

sqlite3 *bdd_open(const char *path) {
  sqlite3 *tmp;
  if(sqlite3_open(path, &tmp)) {
    fprintf(stderr, "Erreur a l'ouverture de la BDD !\n");
    sqlite3_close(tmp);
  }
  return tmp;
}


int bdd_request(sqlite3 *bdd, const char *sql) {
  char *zErrMsg = 0;
  int rc = sqlite3_exec(bdd, sql, callback, 0, &zErrMsg);
  if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL erreur: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
  return rc;
}

int bdd_init(sqlite3 *bdd) {
  sql << "CREATE TABLE IF NOT EXISTS Photoresistance ("	\
    "id INTEGER,"					\
    "valeur INT NOT NULL,"				\
    "date TEXT NOT NULL,"				\
    "PRIMARY KEY(id)"					\
    ");";
  return bdd_request(bdd, sql.str().c_str());
}



/****************************
 * Entree application
 ****************************/

int main(int argc, char** argv){
  string fileBdd;
  bool fileOk = false;
  unsigned long t;

  if(argc == 2 ) {
    bdd = bdd_open(argv[1]);
    if(bdd == NULL) {
      cout << "Erreur BDD! \n" << endl;
      return 0;
    }
  }
  else {
    while(!fileOk) {
      cout << "Entrez le chemin vers une BDD(existante ou a creeer) [*.bd] : ";
      getline(cin, fileBdd);
      if(fileBdd.substr(fileBdd.find_last_of('.')) == ".bd")
	fileOk = true;
    }
    bdd = bdd_open(fileBdd.c_str());
  }
  if(bdd_init(bdd)) {
    cout << "Erreur a l'initialisation !\n" << endl;
  }
  else {
    cout << "BDD initialisee !\n" << endl;
  }
  setup();
  while (1) loop();
  sqlite3_close(bdd);
  return 0;
}
