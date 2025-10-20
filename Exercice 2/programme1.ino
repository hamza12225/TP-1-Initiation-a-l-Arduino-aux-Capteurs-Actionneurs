#include <Arduino.h>

int led = 13;
char octet_rx = 0;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    octet_rx = Serial.read();
    
    if (octet_rx == 'a') {
      digitalWrite(led, HIGH);
      Serial.println("LED allumee");
    }
    
    if (octet_rx == 'e') { 
      digitalWrite(led, LOW);
      Serial.println("LED eteinte");
    }
    
    if (octet_rx == 'i') {
      digitalWrite(led, !digitalRead(led)); 
      if (digitalRead(led) == HIGH) {
        Serial.println("LED inversee -> allumee");
      } else {
        Serial.println("LED inversee -> eteinte");
      }
    }
    
    if (octet_rx == 'c') { 
      Serial.println("LED clignote");
      for(int i = 0; i < 5; i++) {
        digitalWrite(led, LOW);
        delay(500);
        digitalWrite(led, HIGH);
        delay(500);
      }
      digitalWrite(led, LOW);
    }
  }
}