#include <Arduino.h>
#include "ESP32_MailClient.h"

#define WIFI_SSID "WIRELESS NAME"
#define WIFI_PASSWORD "WIRELESS PASSWORD"

#define RXD2 16
#define TXD2 17

char str[21];

//Initializare obiect smtpData
SMTPData smtpData;

//Functie ce returneaza status-ul trimiterii email-ului
void sendCallback(SendStatus info);


void setup()
{

  Serial.begin(115200);

  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial Txd is on pin: "+String(TX));
  Serial.println("Serial Rxd is on pin: "+String(RX));
  Serial.println();

  Serial.print("Conectare la internet..");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

}

void loop()
{

  int i=0;

  if (Serial2.available()) {
    delay(100); //allows all serial sent to be received together
    while(Serial2.available() && i<21) {
      str[i++] = Serial2.read();
    }
    str[i++]='\0';
  }

  if(i>0) {
    Serial.println("Preluare date...");

    preluareDate(String(str));
  }

}

void trimiteEmail(String m, String t, String u, String f)
{

  String miscare = m;
  String temperatura = t;
  String umiditate = u;
  String fum = f;
 
  Serial.println("Trimite email...");

  //Definire client e-mail
  smtpData.setLogin("outlook.office365.com", 587, "email@hotmail.com", "password@hotmail.com");

  //Definire expeditor
  smtpData.setSender("ESP32", "receiver@hotmail.com");

  //Setare prioritate trimitere e-mail
  smtpData.setPriority("High");

  //Definire Subiect
  smtpData.setSubject("DATE CAMERA SERVER");

  //Contoinut email
  smtpData.setMessage("<div style=\"color:#ff0000;font-size:20px;\">Sezorii din camera de server au urmatoarele valori <br/><br/> Detectie Miscare:" + miscare + "<br/>Temperatura: " + temperatura + " C <br/> Umiditatea: " + umiditate + " % <br/> Nivel gaze: " + fum + "   </div>", true);

  //Adresa de livrare
  smtpData.addRecipient("viorelpatru.p@gmail.com");

  smtpData.setSendCallback(sendCallback);

  // Incepere trimiterea email-ului
  if (!MailClient.sendMail(smtpData))
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());

  //Golire memorie
  smtpData.empty();
}

void sendCallback(SendStatus msg)
{
  //Printare status curent
  Serial.println(msg.info());

  //Afisare mesaj de trimitere cu succes!
  if (msg.success())
  {
    Serial.println("----------------");
  }
}


void preluareDate(String d)
{

    String date = d;
   
    String miscare = date.substring(2,3);
    String temperatura = date.substring(4,8);
    String umiditate = date.substring(9,13);
    String fum = date.substring(14,17);


    Serial.println(miscare);
    Serial.println(temperatura);
    Serial.println(umiditate);
    Serial.println(fum);
    Serial.println();
   
    trimiteEmail(miscare, temperatura, umiditate, fum);

    delay(120000);
   
   
}