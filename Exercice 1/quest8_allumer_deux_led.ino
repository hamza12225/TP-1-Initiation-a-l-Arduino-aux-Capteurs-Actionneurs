/**
 * @file led_control.ino
 * @brief Programme pour contrôler 2 LEDs avec des périodes saisies par l'utilisateur
 */

int led1 = 8;
int led2 = 9;
unsigned long periode1 = 1000;
unsigned long periode2 = 1000;
unsigned long dernierTemps1 = 0;
unsigned long dernierTemps2 = 0;
bool stateLed1 = LOW;
bool stateLed2 = LOW;

#define MAX_PERIODE 4294967295UL  // valeur max pour unsigned long

// FSM States
enum Etat { ATTENTE, LECTURE, CLIGNOTEMENT };
Etat etat = ATTENTE;

/**
 * @brief Lire les périodes depuis le port série et vérifie leur validité
 */
void lirePeriodes() {
  if (Serial.available() > 0) {
    long p1 = Serial.parseInt(); // parseInt retourne long
    long p2 = Serial.parseInt();

    // vider le buffer
    while (Serial.available() > 0) Serial.read();

    // vérifier que les valeurs sont valides
    if (p1 <= 0 || p2 <= 0 || p1 > MAX_PERIODE || p2 > MAX_PERIODE) {
      Serial.println("Erreur: valeurs invalides ou trop grandes");
      periode1 = 1000;
      periode2 = 1000;
      etat = ATTENTE;
      return;
    }

    // assigner en toute sécurité aux unsigned long
    periode1 = (unsigned long)p1;
    periode2 = (unsigned long)p2;

    Serial.print("P1 = ");
    Serial.print(periode1);
    Serial.print(" ms, P2 = ");
    Serial.print(periode2);
    Serial.println(" ms");

    etat = CLIGNOTEMENT;
  }
}

/**
 * @brief Mettre à jour l'état d'une LED selon sa période
 */
void updateLed(int pin, unsigned long &dernierTemps, unsigned long periode, bool &state) {
  if (millis() - dernierTemps >= periode) {
    state = !state;
    digitalWrite(pin, state);
    dernierTemps = millis();
  }
}

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  Serial.begin(9600);
  Serial.println("Entrez P1 et P2 (ms) separes par un espace:");
}

void loop() {
  switch (etat) {
    case ATTENTE:
      if (Serial.available() > 0) etat = LECTURE;
      break;

    case LECTURE:
      lirePeriodes();
      break;

    case CLIGNOTEMENT:
      updateLed(led1, dernierTemps1, periode1, stateLed1);
      updateLed(led2, dernierTemps2, periode2, stateLed2);
      if (Serial.available() > 0) etat = LECTURE; // relecture si nouvelle saisie
      break;
  }
}

