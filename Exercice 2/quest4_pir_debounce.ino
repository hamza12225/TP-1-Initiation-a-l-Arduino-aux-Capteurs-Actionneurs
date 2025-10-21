/**
 * @file PIR_Debounce.ino
 * @brief Exemple simple pour illustrer le "debounce" d'un capteur PIR.
 * 
 * Le programme allume la LED seulement si le PIR reste stable (HIGH)
 * pendant un certain temps, pour éviter les faux déclenchements.
 */

// === Constantes ===
#define PIR_PIN 2        // Entrée du capteur PIR
#define LED_PIN 8        // Sortie LED témoin
#define DEBOUNCE_DELAY 150   // Délai de validation du signal (ms)

// === Variables globales ===
bool pirState = LOW;            // État actuel du capteur
bool lastStableState = LOW;     // Dernier état validé (stable)
unsigned long lastChangeTime = 0;  // Dernière fois que le signal a changé

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);

  Serial.println("=== Démonstration PIR avec debounce ===");
  Serial.println("La LED s'allume seulement si le mouvement est stable");
}

void loop() {
  bool currentState = digitalRead(PIR_PIN); // Lire l'état du PIR

  // Si l'état a changé, on démarre le compteur de debounce
  if (currentState != pirState) {
    pirState = currentState;
    lastChangeTime = millis();
  }

  // Vérifier si le nouvel état est resté stable assez longtemps
  if ((millis() - lastChangeTime) > DEBOUNCE_DELAY) {
    if (lastStableState != pirState) {
      lastStableState = pirState;

      if (lastStableState == HIGH) {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("Mouvement détecté (après debounce)");
      } else {
        digitalWrite(LED_PIN, LOW);
        Serial.println("Fin de détection");
      }
    }
  }
}

