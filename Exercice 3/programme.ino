#include <Servo.h>

Servo myServo;

void setup() {
  Serial.begin(9600);
  myServo.attach(9); // SER1 sur shield
  Serial.println("Servo SG90 SER1 : tapez 0 à 6 pour changer la position");
  Serial.println("Positions : 0°, 30°, 60°, 90°, 120°, 150°, 180°");
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();

    switch (command) {
      case '0': myServo.write(0); Serial.println("Position 0°"); break;
      case '1': myServo.write(30); Serial.println("Position 30°"); break;
      case '2': myServo.write(60); Serial.println("Position 60°"); break;
      case '3': myServo.write(90); Serial.println("Position 90°"); break;
      case '4': myServo.write(120); Serial.println("Position 120°"); break;
      case '5': myServo.write(150); Serial.println("Position 150°"); break;
      case '6': myServo.write(180); Serial.println("Position 180°"); break;
      default: Serial.println("Commande invalide : tapez 0 à 6"); break;
    }
  }
}