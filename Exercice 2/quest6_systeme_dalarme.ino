/**
 * @file PIR_AlarmSystem.ino
 * @brief Systeme d'alarme avec capteur PIR, LED et buzzer
 * 
 * Fonctionnalités :
 * - Arm/Disarm via Serial : 'A' = armer, 'D' = desarmer
 * - Anti-spam : le buzzer ne sonne que pendant une durée fixe
 */

#define LED_PIN       4   ///< Pin de la LED témoin
#define BUZZER_PIN1   3   ///< Pin du buzzer actif 1
#define BUZZER_PIN2   5   ///< Pin du buzzer actif 2 (optionnel)
#define PIR_PIN       2   ///< Pin de sortie du capteur PIR
#define BUZZER_DURATION 5000UL ///< Durée du buzzer en ms (anti-spam)

bool systemArmed = true;       ///< Etat du système (armé/désarmé)
int motionDetected = LOW;       ///< Etat actuel du capteur PIR
unsigned long buzzerStart = 0; ///< Timestamp de début d'alarme
bool buzzerActive = false;     ///< Indique si le buzzer est en fonctionnement

// ------------------------------
// Setup des pins et Serial
// ------------------------------
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN1, OUTPUT);
  pinMode(BUZZER_PIN2, OUTPUT);
  pinMode(PIR_PIN, INPUT);

  Serial.begin(9600);
  Serial.println("=== Systeme d'alarme PIR ===");
  Serial.println("Tapez 'A' pour armer, 'D' pour desarmer.");
  delay(5000); // délai pour laisser le capteur se stabiliser
}

// ------------------------------
// Loop principal
// ------------------------------
void loop() {
  // Gestion des commandes série
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == 'A' || cmd == 'a') {
      systemArmed = true;
      Serial.println("Systeme arme");
    } else if (cmd == 'D' || cmd == 'd') {
      systemArmed = false;
      digitalWrite(LED_PIN, LOW);
      digitalWrite(BUZZER_PIN1, LOW);
      digitalWrite(BUZZER_PIN2, LOW);
      buzzerActive = false;
      Serial.println("Systeme desarme");
    }
  }

  // Lecture du PIR
  motionDetected = digitalRead(PIR_PIN);

  // Détection de mouvement si le système est armé
  if (systemArmed && motionDetected == HIGH) {
    if (!buzzerActive) {
      buzzerActive = true;
      buzzerStart = millis();
      Serial.println("Mouvement detecte !");
    }
  }

  // Activation du buzzer et LED
  if (buzzerActive) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN1, HIGH);
    digitalWrite(BUZZER_PIN2, HIGH);

    // Arrêt automatique après la durée définie
    if (millis() - buzzerStart >= BUZZER_DURATION) {
      buzzerActive = false;
      digitalWrite(LED_PIN, LOW);
      digitalWrite(BUZZER_PIN1, LOW);
      digitalWrite(BUZZER_PIN2, LOW);
      Serial.println("Alarme stoppee (anti-spam)");
    }
  } else {
    // S'assurer que tout est éteint si pas de mouvement
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN1, LOW);
    digitalWrite(BUZZER_PIN2, LOW);
  }
}

