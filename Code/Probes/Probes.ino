#include <AccelStepper.h>

// Definir pines
const int Button = A2;
const int Led = A1;
const int En = 10;
const int Stp = 9;
const int Dir = 8;

// Crear instancia de la librería AccelStepper (tipo DRIVER)
AccelStepper stepper(AccelStepper::DRIVER, Stp, Dir);

// Variables de control
bool motorRunning = false;
unsigned long previousMillis = 0;
const long interval = 500;  // Intervalo para el parpadeo del LED (en milisegundos)

void setup() {
  // Configurar pines
  pinMode(Button, INPUT_PULLUP);  // Botón con resistencia pull-up interna
  pinMode(Led, OUTPUT);           // LED
  pinMode(En, OUTPUT);            // Pin de Enable (habilitar motor)
  
  // Configuración inicial del motor
  stepper.setMaxSpeed(5000);      // Establecer velocidad máxima del motor
  stepper.setAcceleration(200);   // Establecer aceleración del motor

  // Inicialmente deshabilitar el motor
  digitalWrite(En, HIGH);         // Deshabilitar el motor (activo en LOW)

  // Indicar que el sistema está listo (LED encendido)
  digitalWrite(Led, HIGH);        // Encender LED de estado
}

void loop() {
  // Leer el estado del botón
  bool buttonState = digitalRead(Button);

  // Si el botón está presionado (LOW), arrancar el motor
  if (buttonState == LOW) {
    if (!motorRunning) {
      // Encender el motor y empezar a girar
      digitalWrite(En, LOW);         // Habilitar el motor (activo en LOW)
      stepper.setSpeed(-4000);         // Velocidad de giro constante
      motorRunning = true;
    }
    
    // Hacer que el motor gire continuamente
    stepper.runSpeed();

    // Parpadear el LED mientras el motor está en marcha
    blinkLed(interval);
    
  } else {
    if (motorRunning) {
      // Detener el motor cuando el botón no esté presionado
      digitalWrite(En, HIGH);         // Deshabilitar el motor
      motorRunning = false;
      
      // Dejar el LED encendido constante cuando el motor está parado
      digitalWrite(Led, HIGH);        // LED constante cuando el motor está parado
    }
  }
}

// Función para parpadear el LED cuando el motor está funcionando
void blinkLed(unsigned long interval) {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Alternar el estado del LED
    int ledState = digitalRead(Led);
    digitalWrite(Led, !ledState);  // Cambiar entre encendido y apagado
  }
}
