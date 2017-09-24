#include <SPI.h>
#include <nRF24L01.h>
#include "RF24.h"

RF24 radio(9,10);
byte addresses[][6] = {"Node1","Node2"};


bool role = 0;



int msg[1];

void setup() {
  // put your setup code here, to run once:
  pinMode(6,OUTPUT); //Received start message);
  pinMode(5,INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  radio.begin();
  Serial.begin(9600);
  Serial.println("Start receiving");
  radio.openReadingPipe(1,addresses[1]);
  radio.openWritingPipe(addresses[0]);
  
  
  radio.startListening();
}

void loop() {

  if (role == 0)
  {
    if (radio.available())
    {
      while (radio.available())
      {
        radio.read(msg,1);
        if (msg[0] == 111) //startup message
        {
          radio.stopListening();
          Serial.println("Sending received message");
          msg[0] = 101;
          radio.write(msg,1);
          radio.startListening();
          Serial.println("Startup please!");
          digitalWrite(6,HIGH);
          delay(1000);
          digitalWrite(6,LOW);
          delay(1000);
          
          role = 1;
          break;
        }
      }
    }    
  }
  else
  {
    
    if (digitalRead(5) == HIGH) // button press will send message
      {
        radio.stopListening(); //stop listening to send message
        Serial.println("Sending message");
        msg[0] = 100;
        radio.write(msg,1);
        digitalWrite(LED_BUILTIN,HIGH);
        delay(1000);
        digitalWrite(LED_BUILTIN,LOW);
        delay(1000);
        radio.startListening();
      }
   
  }

}
