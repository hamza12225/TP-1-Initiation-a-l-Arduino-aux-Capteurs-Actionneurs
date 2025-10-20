/**
 * @file led_control.ino
 * @brief Programme pour controller 2 leds avec des periodes saisies par l'utilisateur
 */

int led1 = 8;
int led2 = 9;
unsigned long periode1 = 1000;
unsigned long periode2 = 1000;
unsigned long dernierTemps1 = 0;
unsigned long dernierTemps2 = 0;
bool stateLed1 = LOW;
bool stateLed2 = LOW;

// FSM States
enum Etat { ATTENTE, LECTURE, CLIGNOTEMENT };
Etat etat = ATTENTE;

/**
 * @brief Lis les periodes
 */
void lirePeriodes() {
  if (Serial.available() > 0) {
    periode1 = Serial.parseInt();
    periode2 = Serial.parseInt();
    while (Serial.available() > 0) Serial.read();

    if (periode1 <= 0 || periode2 <= 0) {
      Serial.println("Erreur: valeurs invalides");
      periode1 = 1000;
      periode2 = 1000;
      etat = ATTENTE;
      return;
    }

    Serial.print("P1 = ");
    Serial.print(periode1);
    Serial.print(" ms, P2 = ");
    Serial.print(periode2);
    Serial.println(" ms");

    etat = CLIGNOTEMENT;
  }
}

/**
 * @brief Metre a jour l'etat d'une LED selon sa periode
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
  Serial.begin(9600);

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

