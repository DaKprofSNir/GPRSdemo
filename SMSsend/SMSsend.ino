// Envoyer un seul SMS
// on ajoute un numero de message pour distinguer le dernier du precedent.

#include <SoftwareSerial.h>
#include <EEPROM.h>
#define GPRSPEED  9600
//#define WaitOK(n) delay(n)
#define WaitOK(n) {delay(n); while( GSMial.available() ) Serial.write((byte)GSMial.read());}
SoftwareSerial GSMial(7, 8);


void setup(){
    GSMial.begin(GPRSPEED);
    Serial.begin(9600);
    Serial.println("SMS send");
    //----- Récupérer le nombre de messages déjà émis
    byte nbMSG=0;
    nbMSG = EEPROM.read( 42 ); // le nbr de SMS envoyés est à l'adresse (arbitraire) 42 dans l'eeprom.
    //----- Paramétrages du modem
    GSMial.print("AT\r");           // un petit OK pour commencer
    WaitOK(1000);                   // Attendre que le modem renvoie "OK" ET afficher cette réponse
    GSMial.print("AT+CMGF=1\r");    // Se mettre en mode texte
    WaitOK(1000);
    //GSMial.print("AT+CSCA=\"+nnnnnnnnnnn\"\r");  // avec nnnn = n° centre de message. C'est inutile pour free
    
    //----- Début envoi du SMS
    GSMial.print("AT+CMGS=\"+33nnnnnnnnn\"\r");  //le numero de telephone cible. Remplacez les n par les 9 derniers chiffres de votre 06
    GSMial.print("-->");GSMial.print(nbMSG);GSMial.print("<--\n\r"); //pour debug : voir le nbr de messages envoyes
    GSMial.print("Meilleurs voeux\n\r Limayrac 2019\n\r");   //le message
    GSMial.write(0x1A);  // Ctrl+Z 
    WaitOK(5000);
    //---- Fin envoi du SMS

    //---- Mise à jour du nombre de messages envoyés
    nbMSG++;
    EEPROM.write(42,nbMSG);
}

// on ne fait rien dans le loop parcequ'on ne veux envoyer qu'un seul SMS à la fois pour ne pas exploser le forfait
void loop(){}

