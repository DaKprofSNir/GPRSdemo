// Recevoir des SMS pour alumer/eteindre la LED
// taille max du SMS 32 octets
// Au moment la reception le modem envoie spontanément <+CMTI: "SM",X> avec X numéro du SMS reçu
// ensuite c'est a vous de demander sa lecture

/*  Exemple de SMS
    +CMGR: "REC UNREAD","+33667944782","","19/01/04,22:35:03+04"
    1iere ligne du SMS
    2ieme ligne du SMS
    Led=0 pour eteindre la LED
    Led=1 pour allumer la LED
    etc
*/

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

#include <SoftwareSerial.h>
#include <EEPROM.h>
#define GPRSPEED  9600
#define WaitOK(n) {delay(n); while( GSMial.available() ) Serial.write((byte)GSMial.read());}

SoftwareSerial GSMial(7, 8);
String msg = String("");

void setup(){
    pinMode(13,OUTPUT); // la sortie que l'on veux commander a distance
    digitalWrite(13,LOW);
    GSMial.begin(GPRSPEED);
    Serial.begin(9600);
    Serial.println("SMS receive");
    //---- parametrage du modem
    GSMial << "AT\r";          // un petit OK pour commencer
    WaitOK(1000);              // Attendre que le modem renvoie "OK" ET afficher cette reponse
    GSMial << "AT+CMGF=1\r";   // Se mettre en mode texte
    WaitOK(1000);
    GSMial << "AT+CMGD=1,4\r"; // effacer la liste des SMS de la memoire du modem
    WaitOK(1000);
}//END setup()


void loop(){
    if(GSMial.available()){
        char recu = GSMial.read();
        //---- ajouter le caractere recu du modem sans les CR ni les LF
        if( recu!='\n' && recu!='\r' ) msg += String(recu);
        
        //---- apres chaque fin de ligne LF, on analyse 
        if( recu=='\n' ){ 
            // echo de la ligne recue vers le moniteur serie
            Serial << "<" << msg << ">\n";
            //--- SMS recu ? ---- on s'attend a qq chose comme <+CMTI: "SM",6>
            if( msg.indexOf( "+CMTI" ) >= 0 ){
                // on se cale sur la virgule pour extraire le n° du message
                String n = msg.substring( 1+msg.indexOf(",") );
                // echo de l'interpretation
                Serial << "*** SMS recu n=" << n << " ***\n\r";
                // on provoque la lecture du SMS par une commande AT
                GSMial << "AT+CMGR=" << n << "\r";
            }//END--- SMS recu
            
            //--- annonce de l'entete du SMS ? On attend qq chose comme <+CMGR: "REC UNREAD","+330000000","","19/01/04,23:34:48+04">
            if( msg.indexOf( "+CMGR:" ) >= 0 ){
                // rien de plus a faire
                // c'etait pour le fun
            }//END--- annonce de l'entete du SMS

            //---- commande de la LED ? On attend qq chose comme <Led=0> ou <Led=1>
            if( msg.indexOf( "Led=" ) >= 0 ){
                String n = msg.substring( 1+msg.indexOf("=") );
                // echo de l'interpretation
                Serial << "*** Commande Led=" << n << " ***\n\r";
                // traitement de l'information
                switch( (n.toInt() ){
                case 0 : digitalWrite(13,LOW ); break;
                case 1 : digitalWrite(13,HIGH); break;
                default: Serial << "*** commande LED non retenue ***\n\r";
                }
            }//END---- commande de la LED
            
            // effacement de la ligne traitee
            msg="";
        }//END if(recu=='\n')
    }//END if(GSMial.available())
}//END loop()
 
 
