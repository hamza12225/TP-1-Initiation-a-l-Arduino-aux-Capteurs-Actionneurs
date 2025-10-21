/**
 * @file FanControlPWM.ino
 * @brief Programme pour controler la vitesse d un mini ventilo avec le PWM 
 *        en utilisant le Adafruit Motor Shield.
 */

#include <AFMotor.h>   // librairie pour controler le moteur DC

AF_DCMotor moteurVentilo(1);  // moteur branché sur le port M1 du shield

/**
 * @brief initialisation du programme
 */
void setup() {
  Serial.begin(9600);
  Serial.println("Tapez une valeur entre 0 et 255 pour regler la vitesse");

  moteurVentilo.setSpeed(0);    // vitesse initial a 0
  moteurVentilo.run(FORWARD);   // rotation avant
}

/**
 * @brief change la vitesse du moteur selon la valeur entrée par l'user
 * @param vitesse valeur PWM entre 0 et 255
 */
void setMotorSpeed(int vitesse) {
  vitesse = constrain(vitesse, 0, 255);   // clamp la valeur entre 0 et 255
  moteurVentilo.setSpeed(vitesse);
  moteurVentilo.run(FORWARD);

  Serial.print("vitesse du moteur : ");
  Serial.println(vitesse);
}

/**
 * @brief boucle principale qui lit la valeur tapee et ajuste la vitesse
 */
void loop() {
  if (Serial.available() > 0) {
    String userInput = Serial.readStringUntil('\n');
    userInput.trim();              
    int val = userInput.toInt(); 
    setMotorSpeed(val);          
  }
}

