#include <Arduino.h>


// Broches des LEDs
const int LED1 = 13;
const int LED2 = 12;

// Variables pour gérer les périodes
unsigned long periode1 = 1000;  // P1 en ms (défaut 1s)
unsigned long periode2 = 500;   // P2 en ms (défaut 0.5s)

// Variables pour le timing (millis)
unsigned long dernierChangement1 = 0;
unsigned long dernierChangement2 = 0;
boolean etat_led1 = false;
boolean etat_led2 = false;

// Variables pour la saisie utilisateur
String inputBuffer = "";
int etape_saisie = 0;  // 0: attente, 1: saisie P1, 2: saisie P2

void setup() {
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  
  Serial.println("=== Controleur 2 LEDs ===");
  Serial.println("Tapez 'start' pour demarrer la saisie");
  Serial.println("Limites: 50ms - 10000ms");
}

void loop() {
  // Gestion des entrées série
  if (Serial.available() > 0) {
    handleSerialInput();
  }
  
  // Clignotement LED1 avec période P1
  if (etape_saisie == 0) {
    clignote(LED1, periode1, &etat_led1, &dernierChangement1);
    clignote(LED2, periode2, &etat_led2, &dernierChangement2);
  }
}

// Fonction pour traiter les entrées série
void handleSerialInput() {
  char caractere = Serial.read();
  
  // Traiter à la réception d'Enter
  if (caractere == '\n' || caractere == '\r') {
    Serial.println();
    if (inputBuffer.length() > 0) {
      traiterCommande();
    }
    inputBuffer = "";
    return;
  }
  
  // Accumuler les caractères valides (ASCII 32-126)
  if (caractere >= 32 && caractere <= 126) {
    inputBuffer += caractere;
    Serial.print(caractere);  // Echo
  }
}

// Fonction pour traiter les commandes
void traiterCommande() {
  inputBuffer.toLowerCase();
  inputBuffer.trim();
  
  if (etape_saisie == 0) {
    if (inputBuffer == "start") {
      Serial.println("Saisie des periodes");
      Serial.print("P1 (50-10000ms): ");
      etape_saisie = 1;
    } else {
      Serial.println("Commande inconnue. Tapez 'start'");
    }
  }
  else if (etape_saisie == 1) {
    if (validerPeriode(inputBuffer, &periode1)) {
      Serial.print("P1 = ");
      Serial.print(periode1);
      Serial.println("ms OK");
      Serial.print("P2 (50-10000ms): ");
      etape_saisie = 2;
    } else {
      Serial.print("Valeur invalide! P1 (50-10000ms): ");
    }
  }
  else if (etape_saisie == 2) {
    if (validerPeriode(inputBuffer, &periode2)) {
      Serial.print("P2 = ");
      Serial.print(periode2);
      Serial.println("ms OK");
      Serial.println("=== Clignotement demarre ===");
      Serial.print("LED1 (pin ");
      Serial.print(LED1);
      Serial.print(") P1=");
      Serial.print(periode1);
      Serial.println("ms");
      Serial.print("LED2 (pin ");
      Serial.print(LED2);
      Serial.print(") P2=");
      Serial.print(periode2);
      Serial.println("ms");
      etape_saisie = 0;
      
      // Réinitialiser les timers
      dernierChangement1 = millis();
      dernierChangement2 = millis();
    } else {
      Serial.print("Valeur invalide! P2 (50-10000ms): ");
    }
  }
}

// Valider et convertir la période
boolean validerPeriode(String input, unsigned long *periode) {
  // Vérifier que ce ne sont que des chiffres
  for (int i = 0; i < input.length(); i++) {
    if (!isDigit(input[i])) {
      return false;
    }
  }
  
  // Convertir en nombre
  long valeur = input.toInt();
  
  // Vérifier les limites (50ms à 10000ms)
  if (valeur >= 50 && valeur <= 10000) {
    *periode = valeur;
    return true;
  }
  
  return false;
}

// Fonction de clignotement sans delay (utilise millis)
void clignote(int broche, unsigned long periode, boolean *etat, unsigned long *lastTime) {
  unsigned long maintenant = millis();
  
  // Inverser l'état tous les (période/2) ms
  if (maintenant - *lastTime >= periode / 2) {
    *etat = !(*etat);
    digitalWrite(broche, *etat ? HIGH : LOW);
    *lastTime = maintenant;
  }
}