//Serial Relay - Lien série à 9600bps entre le PC et la platine empilable GPRS
//
// GPRS Shield est connecté via Software UART 
// Moniteur série est en 9600,8,N avec CRLF en fin de ligne
// Appuyez 2 secondes sur le BP du GPRS pour allumer (comme pour un vrai téléphone)

// La vitesse par défaut des cartes GPRS est souvent 19200
#define GPRSPEED  9600

/* commande      effet <réponses du modem entre chevrons>
 AT              renvoie <OK>
 AT+IPR=?        liste des vitesses disponibles
 AT+IPR=9600     fixe la comm à 9600 et sauve dans la flash du SIMx00
 AT+CSQ          renvoie le niveau de qualité du signal enre 0 et 32, puis le % du taux d'erreur
 AT+CCLK?        renvoie l'heure de la RTC du SIMx00 (ou du temps de mise sous tension si pas de pile
 AT+CCLK=<time>  pour mettre à l'heure
 AT+CREG?        renvoie <+CREG: 0,1> si connecté au réseau
 AT+CGMI         renvoie le nom du constructeur du modem, ici <SIMCOM_Ltd>
 AT+CGSN         renvoie le code IMEI, pour moi "866771026531813"
 AT+COPN         renvoie la liste des opérateurs
 AT+COPS?        renvoie l'opérateur connecté. chez moi c'est : <+COPS: 0,0,"F - Contact">
 AT+CMGR=x       renvoie le SMS n°x
 AT+CMGD=1,4     efface tous les SMS reçus
 réception d'un SMS
 le modem envoie spontanément <+CMTI: "SM",X> avec X numéro du SMS reçu
*/

#include <SoftwareSerial.h>
SoftwareSerial GSMial(7, 8);

void setup(){
	GSMial.begin(GPRSPEED);
	Serial.begin(9600);
    Serial.println("GPRS <-> PC");
}

void loop(){
  if( Serial.available() ) GSMial.write((byte)Serial.read()); //si besoin du ^z préférer un vrai terminal
  if( GSMial.available() ) Serial.write((byte)GSMial.read());
}
