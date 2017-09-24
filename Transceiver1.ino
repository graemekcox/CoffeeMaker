#include <SPI.h>
#include <nRF24L01.h>
#include "RF24.h"

RF24 radio(9,10);
byte addresses[][6] = {"Node1","Node2"};


bool role = 1; //start transmitting



int msg[1];

void setup() {
  // put your setup code here, to run once:
  pinMode(6,INPUT);
  pinMode(5,OUTPUT);  //Coffee is ready
  pinMode(4,OUTPUT);  // Coffee is cold
  pinMode(LED_BUILTIN, OUTPUT);

  radio.begin();
  Serial.begin(9600);

  Serial.println("Start transmitting");
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1,addresses[0]);

  radio.startListening();
}

void loop() {
  // role == 1 is transmitting
  if (role == 1)
  {
    boolean timeout = false;

    if (digitalRead(6) == HIGH) // button press will send message
    {
      radio.stopListening(); //stop listening to send message
      Serial.println("Sending message");
      msg[0] = 111;
      radio.write(msg,1);
      radio.startListening();
      
      //digitalWrite(LED_BUILTIN,HIGH);
      //delay(1000);
      //digitalWrite(LED_BUILTIN,LOW);
      //delay(1000);
      
      
      unsigned long start_time = micros();
      
      while (!radio.available()){
        if (micros()-start_time > 20000000)
        {
          timeout = true;
          break;
        }
      }
      if (timeout)
      {
        Serial.println("Failed to get a response");
      }
      else
      {
        
        radio.read(msg,1);
        Serial.println(msg[0]);
        if (msg[0] == 101)
        {
          Serial.println("Received confirmed message. Time to start listening");
          role = 0;
        }
      }
    }

      
    
  }
  else //role 0. Now wait for responses
  {
    if (radio.available())
    {
      while (radio.available())
      {
        radio.read(msg,1);
        //Serial.println(msg[0]

        if (msg[0] == 100) //Coffees ready!
        {
          Serial.println("Coffee is ready!");
          digitalWrite(5,HIGH);
          delay(1000);
          digitalWrite(5,LOW);
          delay(1000);
        }
        else if (msg[0] == 110)
        {
          Serial.println("Coffee is getting cold!");
          digitalWrite(4,HIGH);
          delay(1000);
          digitalWrite(4,LOW);
          delay(1000);
        }
      }
    }
  }
}
