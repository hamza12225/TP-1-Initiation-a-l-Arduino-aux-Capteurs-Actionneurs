/**
 * @file ServoControl.ino
 * @brief Controle d’un servo 
* L’utilisateur tape un chiffre de 1 a 7 pour faire tourner le servo 
 */

#include <Servo.h>  ///Librairie Arduino pour controler servos

// -----------------------------------------------------------------------------
// Constantes
// -----------------------------------------------------------------------------
#define SERVO_PIN  9   ///Broche du servo (SER1 sur le shield)

// -----------------------------------------------------------------------------
// Objet global
// -----------------------------------------------------------------------------
Servo servoMoteur;   ///Objet servo global pour controler le SG90

// -----------------------------------------------------------------------------
// Initialisation
// -----------------------------------------------------------------------------
/**
 * @brief Initialisation la communication serie et le servo
 */
void setup() {
  Serial.begin(9600);
  servoMoteur.attach(SERVO_PIN);
  Serial.println("Tapez un chiffre entre 1 et 7 pour choisir la position");
  Serial.println("Positions: 0, 30, 60, 90, 120, 150, 180 degres");
}

// -----------------------------------------------------------------------------
// Boucle principale
// -----------------------------------------------------------------------------
/**
 * @brief Lecture des commandes serie et mouvement du servo selon la valeur
 */
void loop() {
  if (Serial.available() > 0) {
    char commande = Serial.read();  
    // Ignore les retours chariots
    if (commande == '\r' || commande == '\n') return;

    // Selection de la position selon la commande
    switch (commande) {
      case '0': servoMoteur.write(0);   Serial.println("0 degres"); break;
      case '1': servoMoteur.write(30);  Serial.println("30 degres"); break;
      case '2': servoMoteur.write(60);  Serial.println("60 degres"); break;
      case '3': servoMoteur.write(90);  Serial.println("90 degres"); break;
      case '4': servoMoteur.write(120); Serial.println("120 degres"); break;
      case '5': servoMoteur.write(150); Serial.println("150 degres"); break;
      case '6': servoMoteur.write(180); Serial.println("180 degres"); break;
      default:
        Serial.println("Commande invalide! Tapez entreq 1 et 7");
        break;
    }
  }
}

