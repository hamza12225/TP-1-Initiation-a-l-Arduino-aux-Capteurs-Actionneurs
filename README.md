
# TP 1 : Initiation à l'Arduino, aux Capteurs et aux Actionneurs

[![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white)](https://www.arduino.cc/)
[![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)](LICENSE)

> Travaux pratiques réalisés dans le cadre du cours d'Arduino à l'INPT sous la supervision du **Pr. Yann BEN MAISSA**

## 📋 Table des matières

- [À propos](#-à-propos)
- [Équipe](#-équipe)
- [Matériel requis](#-matériel-requis)
- [Structure du projet](#-structure-du-projet)
- [Exercices](#-exercices)
  - [Exercice 1 : Communication Série](#exercice-1--communication-série-et-contrôle-de-led)
  - [Exercice 2 : Capteur PIR](#exercice-2--capteur-pir-et-système-dalarme)
  - [Exercice 3 : Contrôle de moteurs](#exercice-3--contrôle-de-moteurs)
- [Installation](#-installation)
- [Utilisation](#-utilisation)
- [Méthodologie Agile](#-méthodologie-agile)
- [Schémas de montage](#-schémas-de-montage)
- [Ressources](#-ressources)
- [Licence](#-licence)

---

## 🎯 À propos

Ce projet constitue une introduction pratique à l'écosystème Arduino, couvrant :

- 📡 La communication série UART
- 🔍 L'utilisation de capteurs (PIR, ultrasons, niveau d'eau)
- ⚙️ Le contrôle d'actionneurs (LEDs, moteurs DC, servomoteurs, moteurs pas à pas)
- 🛠️ Les bonnes pratiques de développement embarqué
- 📊 La gestion de projet Agile avec Scrum

## 👥 Équipe

| Nom | Rôle |
|-----|------|
| **Hamza ELHAIKI** | Développeur & Scrum Master |
| **Youssef BIKOUCH** | Développeur |
| **Youssef MAHDI** | Développeur |
| **Yassine KEROUMI** | Développeur |

## 🔧 Matériel requis

### Cartes de développement
- **Arduino Uno** (ATmega328P)
- **Raspberry Pi 4** (BCM2711)
- **ESP32** (Espressif)

### Contrôleurs et Drivers
- **L293D Motor Drive Shield** - Contrôle de moteurs DC

### Capteurs
- **HC-SR04** - Capteur ultrason (2-400 cm)
- **HW-038** - Capteur de niveau d'eau
- **PIR Motion Sensor** - Détection de mouvement

### Actionneurs
- **Moteur DC** avec ventilateur
- **Servomoteur SG90** (0-180°)
- **Moteur pas à pas 28BYJ-48**
- **LEDs** (rouge, verte, jaune)
- **Buzzer** actif

### Accessoires
- Breadboard
- Fils de connexion (mâle-mâle, mâle-femelle)
- Résistances (220Ω, 10kΩ)
- Câbles USB (Type-B, Micro-USB)

## 📁 Structure du projet

```
TP-1-Initiation-a-l-Arduino-aux-Capteurs-Actionneurs/
│
├── Exercice_1_Communication_Serie/
│   ├── led_serial_control/
│   │   └── led_serial_control.ino
│   └── README.md
│
├── Exercice_2_Capteur_PIR/
│   ├── PIR_ModeDemo/
│   │   └── PIR_ModeDemo.ino
│   ├── PIR_Debounce/
│   │   └── PIR_Debounce.ino
│   ├── PIR_AlarmSystem/
│   │   └── PIR_AlarmSystem.ino
│   └── README.md
│
├── Exercice_3_Controle_Moteurs/
│   ├── MiniVentilateur/
│   │   └── MiniVentilateur.ino
│   ├── ServoControl/
│   │   └── ServoControl.ino
│   ├── StepperControl/
│   │   └── StepperControl.ino
│   └── README.md
│
├── docs/
│   ├── schemas/
│   ├── datasheets/
│   └── rapport.pdf
│
├── images/
│   ├── montages/
│   └── architecture/
│
├── README.md
└── LICENSE
```

---

## 🔬 Exercices

### Exercice 1 : Communication Série et Contrôle de LED

#### 🎯 Objectifs
- Comprendre la communication USB-UART sur Arduino
- Maîtriser le port série (Serial)
- Contrôler des LEDs via des commandes texte

#### 📌 Concepts clés

**Communication USB-UART**
- L'Arduino Uno utilise l'**ATmega16U2** comme pont USB-UART
- Convertit les données USB du PC en signaux série UART pour l'ATmega328P
- Baud rate : synchronisation essentielle entre émetteur/récepteur (défaut: 9600)

**Caractéristiques des LEDs**
- Tension directe (Vf) :
  - Rouge : 1.8-2.0V
  - Verte : 2.0-3.3V
  - Bleue/Blanche : 3.0-3.6V
- Courant nominal : 10-20 mA
- ⚠️ **Toujours utiliser une résistance de limitation**

**Calcul de résistance**
```
R = (Vcc - Vf) / I

Exemple : LED verte (Vf=2.2V), Vcc=5V, I=15mA
R = (5 - 2.2) / 0.015 = 186.7Ω ≈ 220Ω
```

#### 💻 Code principal

```cpp
/**
 * Contrôle de LED via commandes série
 * Commandes : allumer, eteindre, clignoter, inverser
 */

const int LED_PIN = 9;
String commande = "";
bool etatLed = LOW;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Commandes: allumer, eteindre, clignoter, inverser");
}

void loop() {
  if (Serial.available() > 0) {
    char c = Serial.read();
    
    if (c != '\n' && c != '\r') {
      commande += c;
    }
    
    if (c == '\n') {
      commande.trim();
      
      if (commande.equalsIgnoreCase("allumer")) {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("LED allumee");
      }
      else if (commande.equalsIgnoreCase("eteindre")) {
        digitalWrite(LED_PIN, LOW);
        Serial.println("LED eteinte");
      }
      else if (commande.equalsIgnoreCase("clignoter")) {
        for (int i = 0; i < 3; i++) {
          digitalWrite(LED_PIN, HIGH);
          delay(300);
          digitalWrite(LED_PIN, LOW);
          delay(300);
        }
      }
      else if (commande.equalsIgnoreCase("inverser")) {
        etatLed = !etatLed;
        digitalWrite(LED_PIN, etatLed);
      }
      
      commande = "";
    }
  }
}
```

#### 🔌 Schéma de montage

```
Arduino Uno
    Pin 9 ----[220Ω]----[LED]---- GND
```

---

### Exercice 2 : Capteur PIR et Système d'Alarme

#### 🎯 Objectifs
- Comprendre le fonctionnement des capteurs PIR (Passive Infrared)
- Implémenter un système de détection de mouvement
- Créer un système d'alarme complet

#### 📌 Concepts clés

**Fonctionnement du PIR**
- Détecte les variations de rayonnement infrarouge (10 μm)
- Capteur pyroélectrique à deux éléments (configuration différentielle)
- Compensation automatique de la température ambiante
- Circuit BISS0001 pour traitement du signal

**Modes de fonctionnement**

| Mode | Jumper | Comportement |
|------|--------|--------------|
| **Single Trigger** | H | Pulse unique par détection |
| **Repeatable Trigger** | L | Signal continu tant que mouvement |

**Réglages disponibles**
- 🎚️ **Sensibilité** (potentiomètre Sx) : portée de détection (3-7m)
- ⏱️ **Délai** (potentiomètre Tx) : durée du signal HIGH (0.3-300s)

#### 💻 Code : Détection avec debounce

```cpp
/**
 * Détection PIR fiable avec debounce et fenêtre temporelle
 */

#define PIR_PIN 2
#define LED_PIN 8
#define DEBOUNCE_TIME 100
#define TIME_WINDOW 100

bool pirState = false;
bool lastStableState = false;
unsigned long lastChangeTime = 0;

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  delay(5000); // Stabilisation du capteur
}

void loop() {
  bool currentReading = digitalRead(PIR_PIN);
  unsigned long currentTime = millis();
  
  // Debounce
  if (currentReading != pirState) {
    lastChangeTime = currentTime;
    pirState = currentReading;
  }
  
  // Validation après stabilisation
  if ((currentTime - lastChangeTime) >= DEBOUNCE_TIME) {
    if (pirState != lastStableState) {
      lastStableState = pirState;
      
      if (pirState == HIGH) {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("Mouvement detecte");
      } else {
        digitalWrite(LED_PIN, LOW);
        Serial.println("Fin de mouvement");
      }
    }
  }
}
```

#### 💻 Code : Système d'alarme complet

```cpp
/**
 * Système d'alarme avec anti-spam
 * Commandes : 'A' = armer, 'D' = desarmer
 */

#define LED_PIN 4
#define BUZZER_PIN1 3
#define BUZZER_PIN2 5
#define PIR_PIN 2
#define BUZZER_DURATION 5000UL

bool systemArmed = true;
bool buzzerActive = false;
unsigned long buzzerStart = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN1, OUTPUT);
  pinMode(BUZZER_PIN2, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  
  Serial.begin(9600);
  Serial.println("=== Alarme PIR ===");
  Serial.println("'A' = armer | 'D' = desarmer");
  delay(5000);
}

void loop() {
  // Gestion commandes
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == 'A' || cmd == 'a') {
      systemArmed = true;
      Serial.println("Systeme ARME");
    } else if (cmd == 'D' || cmd == 'd') {
      systemArmed = false;
      stopAlarm();
      Serial.println("Systeme DESARME");
    }
  }
  
  // Détection mouvement
  if (systemArmed && digitalRead(PIR_PIN) == HIGH && !buzzerActive) {
    buzzerActive = true;
    buzzerStart = millis();
    Serial.println("ALERTE: Intrusion detectee!");
  }
  
  // Gestion alarme
  if (buzzerActive) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN1, HIGH);
    digitalWrite(BUZZER_PIN2, HIGH);
    
    if (millis() - buzzerStart >= BUZZER_DURATION) {
      stopAlarm();
      Serial.println("Alarme arretee (anti-spam)");
    }
  }
}

void stopAlarm() {
  buzzerActive = false;
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN1, LOW);
  digitalWrite(BUZZER_PIN2, LOW);
}
```

#### 🔌 Schéma de montage

```
PIR Sensor          Arduino Uno
    VCC   ----------- 5V
    GND   ----------- GND
    OUT   ----------- Pin 2
    
LED       ----------- Pin 4 (avec résistance 220Ω)
Buzzer 1  ----------- Pin 3
Buzzer 2  ----------- Pin 5
```

---

### Exercice 3 : Contrôle de Moteurs

#### 🎯 Objectifs
- Contrôler différents types de moteurs
- Maîtriser le PWM pour la variation de vitesse
- Utiliser le Motor Shield L293D

#### 📌 Concepts clés

**L293D Motor Shield**
- 4 ponts en H indépendants
- Courant max : 600mA continu, 1.2A crête
- Tension : 4.5-25V
- Protection thermique intégrée

**Types de moteurs**

| Type | Caractéristiques | Contrôle |
|------|-----------------|----------|
| **DC** | Rotation continue, vitesse variable | PWM (0-255) |
| **Servo** | Position angulaire (0-180°) | Signal PWM calibré |
| **Stepper** | Rotation précise par pas | Séquence de phases |

#### 💻 Code : Moteur DC (ventilateur)

```cpp
/**
 * Contrôle de vitesse d'un ventilateur DC
 */

#include <AFMotor.h>

AF_DCMotor moteurVentilo(1); // Port M1

void setup() {
  Serial.begin(9600);
  Serial.println("=== Ventilateur DC ===");
  Serial.println("Entrez vitesse (0-255)");
  
  moteurVentilo.setSpeed(0);
  moteurVentilo.run(FORWARD);
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    int vitesse = constrain(input.toInt(), 0, 255);
    
    moteurVentilo.setSpeed(vitesse);
    Serial.print("Vitesse: ");
    Serial.println(vitesse);
  }
}
```

#### 💻 Code : Servomoteur

```cpp
/**
 * Contrôle de position d'un servo SG90
 */

#include <Servo.h>

#define SERVO_PIN 9

Servo servo;
const int positions[] = {0, 30, 60, 90, 120, 150, 180};

void setup() {
  Serial.begin(9600);
  servo.attach(SERVO_PIN);
  
  Serial.println("=== Servomoteur ===");
  Serial.println("Tapez 0-6 pour changer position");
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    
    if (cmd >= '0' && cmd <= '6') {
      int index = cmd - '0';
      servo.write(positions[index]);
      
      Serial.print("Position: ");
      Serial.print(positions[index]);
      Serial.println(" degres");
    }
  }
}
```

#### 💻 Code : Moteur pas à pas

```cpp
/**
 * Contrôle d'un stepper 28BYJ-48
 * Commandes : 'f' = avant, 'b' = arriere, nombre = vitesse
 */

#define PIN1 9
#define PIN2 10
#define PIN3 11
#define PIN4 12

bool direction = true;
uint16_t stepDelay = 5;
uint8_t currentStep = 0;

const uint8_t sequence[4][4] = {
  {HIGH, LOW,  LOW,  LOW},
  {LOW,  HIGH, LOW,  LOW},
  {LOW,  LOW,  HIGH, LOW},
  {LOW,  LOW,  LOW,  HIGH}
};

void setup() {
  pinMode(PIN1, OUTPUT);
  pinMode(PIN2, OUTPUT);
  pinMode(PIN3, OUTPUT);
  pinMode(PIN4, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("=== Stepper Motor ===");
  Serial.println("'f'/'b' : direction | nombre : vitesse (ms)");
}

void stepMotor() {
  uint8_t idx = direction ? currentStep : (3 - currentStep);
  
  digitalWrite(PIN1, sequence[idx][0]);
  digitalWrite(PIN2, sequence[idx][1]);
  digitalWrite(PIN3, sequence[idx][2]);
  digitalWrite(PIN4, sequence[idx][3]);
  
  currentStep = (currentStep + 1) % 4;
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    
    if (input == "f") {
      direction = true;
      Serial.println("Direction: AVANT");
    }
    else if (input == "b") {
      direction = false;
      Serial.println("Direction: ARRIERE");
    }
    else {
      stepDelay = constrain(input.toInt(), 3, 1000);
      Serial.print("Vitesse: ");
      Serial.print(stepDelay);
      Serial.println(" ms");
    }
  }
  
  stepMotor();
  delay(stepDelay);
}
```

#### 🔌 Schémas de montage

**Motor Shield**
```
Arduino Uno + L293D Shield
    M1 : Moteur DC (ventilateur)
    SER1 (Pin 9) : Servomoteur
    M3/M4 : Stepper (Pins 9-12)
```

---

## 🚀 Installation

### Prérequis

1. **Arduino IDE** (version 1.8.x ou 2.x)
   ```bash
   # Télécharger depuis
   https://www.arduino.cc/en/software
   ```

2. **Bibliothèques Arduino**
   ```
   - AFMotor (Adafruit Motor Shield)
   - Servo (incluse dans Arduino IDE)
   ```

3. **Drivers USB**
   - CH340/CH341 pour clones Arduino
   - FTDI pour cartes officielles

### Installation des bibliothèques

#### Via le gestionnaire de bibliothèques Arduino
1. Ouvrir Arduino IDE
2. Menu : `Croquis` → `Inclure une bibliothèque` → `Gérer les bibliothèques`
3. Rechercher et installer :
   - `Adafruit Motor Shield Library`

#### Via Git
```bash
cd ~/Documents/Arduino/libraries/
git clone https://github.com/adafruit/Adafruit-Motor-Shield-library.git
```

### Clonage du projet

```bash
git clone https://github.com/hamza12225/TP-1-Initiation-a-l-Arduino-aux-Capteurs-Actionneurs.git
cd TP-1-Initiation-a-l-Arduino-aux-Capteurs-Actionneurs
```

---

## 🎮 Utilisation

### Téléversement d'un programme

1. Connecter l'Arduino via USB
2. Ouvrir le fichier `.ino` dans Arduino IDE
3. Sélectionner :
   - **Carte** : Arduino Uno
   - **Port** : COM3 (Windows) ou /dev/ttyUSB0 (Linux)
4. Cliquer sur `Téléverser` (→)

### Moniteur série

1. Ouvrir : `Outils` → `Moniteur série`
2. Régler le baud rate : **9600 baud**
3. Envoyer des commandes selon l'exercice

### Exemples de commandes

**Exercice 1 (LED)**
```
allumer
eteindre
clignoter
inverser
```

**Exercice 2 (Alarme)**
```
A  → Armer le système
D  → Désarmer le système
```

**Exercice 3 (Moteurs)**
```
# Ventilateur DC
150  → Vitesse moyenne

# Servomoteur
3    → Position 90°

# Stepper
f    → Avant
b    → Arrière
10   → Vitesse 10ms/step
```

---

## 📊 Méthodologie Agile

Ce projet a été développé en suivant la méthodologie **Scrum** :

### 🔄 Organisation

- **Sprints** : 1 sprint par exercice
- **Durée** : 2-3 jours par sprint
- **Outil** : Jira Software

### 📋 Workflow

```mermaid
graph LR
    A[Backlog] --> B[Sprint Planning]
    B --> C[Development]
    C --> D[Code Review]
    D --> E[Testing]
    E --> F[Push GitHub]
    F --> G[Sprint Review]
```

### 🛠️ Outils utilisés

| Outil | Usage |
|-------|-------|
| **Jira** | Gestion des tâches et sprints |
| **Slack** | Communication d'équipe |
| **Git/GitHub** | Contrôle de version |
| **Arduino IDE** | Développement |
| **Fritzing** | Schémas de montage |

### 📈 Répartition des tâches

- **Recherche** : Documentation technique et datasheets
- **Développement** : Programmation Arduino
- **Tests** : Validation des montages et programmes
- **Documentation** : Rédaction du rapport et README

---

## 🖼️ Schémas de montage

### Exercice 1 : LED avec résistance

```
        220Ω
5V ----[====]----[>|]---- GND
        (R)      (LED)
```

### Exercice 2 : Système d'alarme PIR

```
         PIR Sensor
         ┌────────┐
         │  VCC   │─── 5V
         │  GND   │─── GND
         │  OUT   │─── Pin 2
         └────────┘
         
    LED (+220Ω) ─── Pin 4
    Buzzer 1 ─────── Pin 3
    Buzzer 2 ─────── Pin 5
```

### Exercice 3 : Motor Shield

```
Arduino Uno
    │
    ├── L293D Shield
    │   ├── M1 : DC Motor
    │   ├── M2 : [libre]
    │   ├── M3 : Stepper (phase 1-2)
    │   └── M4 : Stepper (phase 3-4)
    │
    └── Pin 9 : Servo SG90
```

---

## 📚 Ressources

### Documentation officielle

- [Arduino Reference](https://www.arduino.cc/reference/en/)
- [Adafruit Motor Shield Guide](https://learn.adafruit.com/adafruit-motor-shield)
- [PIR Sensor Datasheet](https://learn.adafruit.com/pir-passive-infrared-proximity-motion-sensor)

### Datasheets

- [ATmega328P](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)
- [L293D Motor Driver](https://www.ti.com/lit/ds/symlink/l293.pdf)
- [HC-SR04 Ultrasonic](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf)

### Tutoriels

- [Arduino Serial Communication](https://docs.arduino.cc/learn/communication/serial)
- [PWM Explained](https://docs.arduino.cc/learn/microcontrollers/analog-output)
- [Stepper Motor Control](https://docs.arduino.cc/learn/electronics/stepper-motors)

### Outils

- [Arduino IDE](https://www.arduino.cc/en/software)
- [Fritzing](https://fritzing.org/) - Schémas de circuit
- [TinkerCAD](https://www.tinkercad.com/) - Simulation en ligne
- [PlatformIO](https://platformio.org/) - IDE alternatif

---

## 🤝 Contribution

Les contributions sont les bienvenues ! Pour contribuer :

1. Fork le projet
2. Créer une branche (`git checkout -b feature/AmazingFeature`)
3. Commit les changements (`git commit -m 'Add AmazingFeature'`)
4. Push vers la branche (`git push origin feature/AmazingFeature`)
5. Ouvrir une Pull Request

### Standards de code

- Indentation : 2 espaces
- Commentaires en français
- Documentation Doxygen pour les fonctions
- Noms de variables explicites

---

## 🐛 Résolution de problèmes

### Arduino non détecté

```bash
# Linux : ajouter l'utilisateur au groupe dialout
sudo usermod -a -G dialout $USER
sudo reboot

# Vérifier le port
ls /dev/tty*
```

### Erreur de compilation

```
Erreur : 'AF_DCMotor' does not name a type
→ Solution : Installer la bibliothèque AFMotor
```

### PIR toujours actif

1. Attendre 60s pour stabilisation
2. Ajuster le potentiomètre de sensibilité
3. Vérifier le mode jumper (H/L)

### Moteur ne tourne pas

1. Vérifier l'alimentation externe (7-12V)
2. Vérifier les connexions
3. Tester avec vitesse maximale (255)

---

## 📄 Licence

Ce projet est sous licence MIT. Voir le fichier [LICENSE](LICENSE) pour plus de détails.

```
MIT License

Copyright (c) 2025 Hamza ELHAIKI, Youssef BIKOUCH, Youssef MAHDI, Yassine KEROUMI

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction...
```

---

## 👨‍🏫 Encadrement

**Professeur** : Pr. Yann BEN MAISSA  
**Institution** : Institut National des Postes et Télécommunications (INPT)  
**Année académique** : 2024-2025

---

## 📞 Contact

Pour toute question ou suggestion :

- **Hamza ELHAIKI** - [@hamza12225](https://github.com/hamza12225)
- **Projet GitHub** - [TP-1-Initiation-a-l-Arduino](https://github.com/hamza12225/TP-1-Initiation-a-l-Arduino-aux-Capteurs-Actionneurs)

---

## 🌟 Remerciements

- Pr. Yann BEN MAISSA pour l'encadrement
- L'équipe pédagogique de l'INPT
- La communauté Arduino pour la documentation
- Adafruit pour les bibliothèques open-source

---

<div align="center">

**Fait avec ❤️ à l'INPT**

⭐ N'oubliez pas de donner une étoile si ce projet vous a été utile !

</div>
