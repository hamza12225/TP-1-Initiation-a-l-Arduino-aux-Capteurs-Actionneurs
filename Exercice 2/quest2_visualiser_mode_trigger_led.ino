/**
 * @file PIR_ModeDemo.ino
 * @brief Visualisation de l'effet du mode Single vs Repeatable trigger sur un PIR
 */

#define PIR_PIN 2       ///< Sortie du capteur PIR
#define LED_PIN 8       ///< LED témoin

bool pirState = false;
bool lastPirState = false;

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("=== PIR Mode Demo ===");
  Serial.println("LED s'allume selon détection PIR");
  Serial.println("Mode jumper H = single trigger, L = repeatable trigger");
}

void loop() {
  pirState = digitalRead(PIR_PIN);

  if (pirState && !lastPirState) {
    // Détection d'un mouvement
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Mouvement détecté !");
    Serial.println("Observez le comportement selon le mode jumper");
  }

  // Éteindre la LED si le PIR est LOW
  if (!pirState) {
    digitalWrite(LED_PIN, LOW);
  }

  lastPirState = pirState;
}

