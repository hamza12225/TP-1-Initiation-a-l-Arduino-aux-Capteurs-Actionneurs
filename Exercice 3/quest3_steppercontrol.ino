/**
 * @file StepperControl.ino
 * @brief Controle d'un moteur stepper 28BYJ-48 avec vitesse et direction 
 * 
 * L'utilisateur peut entrer :
 * - 'f' ou 'b' pour la direction
 * - un nombre pour la vitesse en millisecondes par step
 */

#define STEPPER_PIN_1 9
#define STEPPER_PIN_2 10
#define STEPPER_PIN_3 11
#define STEPPER_PIN_4 12

#define MIN_STEP_DELAY 3     ///< Minimum delay par step (ms)
#define MAX_STEP_DELAY 1000  ///< Maximum delay par step (ms)

bool stepDirection = true;   ///< true = avant, false = arrière
uint16_t stepDelay = 5;      ///< Delay initial entre chaque step
uint8_t currentStep = 0;     ///< Step actuel (0-3)

String inputString = "";     ///< Stocke l'entrée du serial monitor

// ------------------------------
// Setup des pins
// ------------------------------

/**
 * @brief Configure les pins du moteur comme sorties
 */
void setupStepperPins() {
    pinMode(STEPPER_PIN_1, OUTPUT);
    pinMode(STEPPER_PIN_2, OUTPUT);
    pinMode(STEPPER_PIN_3, OUTPUT);
    pinMode(STEPPER_PIN_4, OUTPUT);
}

// ------------------------------
// Fonction pour faire un step
// ------------------------------

/**
 * @brief Effectuer un step du moteur selon la direction choisie
 * 
 * @param direction true = avant, false = arrière
 */
void stepMotor(bool direction) {
    static const uint8_t stepSequence[4][4] = {
        {HIGH, LOW,  LOW,  LOW},
        {LOW,  HIGH, LOW,  LOW},
        {LOW,  LOW,  HIGH, LOW},
        {LOW,  LOW,  LOW,  HIGH}
    };

    uint8_t stepIndex;

    if (direction) stepIndex = currentStep;
    else stepIndex = 3 - currentStep;

    digitalWrite(STEPPER_PIN_1, stepSequence[stepIndex][0]);
    digitalWrite(STEPPER_PIN_2, stepSequence[stepIndex][1]);
    digitalWrite(STEPPER_PIN_3, stepSequence[stepIndex][2]);
    digitalWrite(STEPPER_PIN_4, stepSequence[stepIndex][3]);

    currentStep = (currentStep + 1) % 4;
}

// ------------------------------
// Lire le serial monitor
// ------------------------------

/**
 * @brief Lit le serial monitor et stocke les caractères jusqu'à '\n' ou '\r'
 */
void serialEvent() {
    while (Serial.available()) {
        char inChar = (char)Serial.read();
        if (inChar != '\n' && inChar != '\r') {
            inputString += inChar;
        } else {
            handleSerialInput(); // Traite l'entrée dès qu'on a un retour à la ligne
        }
    }
}

/**
 * @brief Traite la commande entrée par l'utilisateur
 * 
 * Les commandes possibles :
 * - f : direction avant
 * - b : direction arriere
 * - nombre : vitesse en ms par step
 */
void handleSerialInput() {
    inputString.trim(); // enlever espaces et caractères invisibles

    // Direction
    if (inputString == "f") {
        stepDirection = true;
        Serial.println("Direction: avant");
    } 
    else if (inputString == "b") {
        stepDirection = false;
        Serial.println("Direction: arriere");
    } 
    // Vitesse
    else {
        long val = inputString.toInt(); // convertir en nombre
        if (val < MIN_STEP_DELAY) val = MIN_STEP_DELAY;
        if (val > MAX_STEP_DELAY) val = MAX_STEP_DELAY;

        stepDelay = val;
        Serial.print("Vitesse: ");
        Serial.print(stepDelay);
        Serial.println(" ms par step");
    }

    inputString = ""; // reset pour next input
}

// ------------------------------
// Setup et loop
// ------------------------------

/**
 * @brief Configuration initiale
 * Initialise le serial monitor et les pins du moteur
 */
void setup() {
    Serial.begin(9600);
    Serial.println("Tapez 'f' ou 'b' ou un nombre entre 3 et 1000");
    setupStepperPins();
}

/**
 * @brief Boucle principale
 * Fait tourner le moteur et gerer la vitesse
 */
void loop() {
    stepMotor(stepDirection);
    delay(stepDelay);
}

