/**
 * @file led_serial_control.ino
 * @brief Controle de LED via le port serie avec plusieurs commandes.
 *
 * Commandes disponibles :
 * - allumer   : allume la LED
 * - eteindre  : eteint la LED
 * - clignoter : fait clignoter la LED
 * - inverser  : inverse l'état actuel de la LED
 */

int led = 9;                // broche de la LED
String commande = "";       // chaine pour stocker la commande recue
bool etatLed = LOW;         // mémorise l'état actuel de la LED

void setup() {
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  Serial.begin(9600);
  Serial.println("Pret a recevoir une commande : allumer, eteindre, clignoter, inverser");
}

void loop() {
  if (Serial.available() > 0) {
    char c = Serial.read();

    // ignorer les retours chariot et les espaces
    if (c != '\n' && c != '\r') {
      commande += c;
    }

    // si la commande est complete (fin de ligne)
    if (c == '\n') {
      commande.trim(); // supprime les espaces et retours chariot

      if (commande.equalsIgnoreCase("allumer")) {
        etatLed = HIGH;
        digitalWrite(led, etatLed);
        Serial.println("LED allumee");
      }
      else if (commande.equalsIgnoreCase("eteindre")) {
        etatLed = LOW;
        digitalWrite(led, etatLed);
        Serial.println("LED eteinte");
      }
      else if (commande.equalsIgnoreCase("clignoter")) {
        Serial.println("LED clignote");
        for (int i = 0; i < 3; i++) {
          etatLed = HIGH;
          digitalWrite(led, etatLed);
          delay(300);
          etatLed = LOW;
          digitalWrite(led, etatLed);
          delay(300);
        }
      }
      else if (commande.equalsIgnoreCase("inverser")) {
        etatLed = !etatLed;
        digitalWrite(led, etatLed);
        Serial.println("Etat de la LED inverse");
      }
      else {
        Serial.println("Commande non reconnue");
      }

      commande = ""; // reset pour la prochaine commande
    }
  }
}

