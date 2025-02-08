
#include "SPI.h"
#include "Adafruit_GFX.h"
#include <AccelStepper.h>
#include "Adafruit_ILI9341.h"

// Pines del motor y LEDs
const int dirPin = 8;      // Pin de dirección del motor
const int stepPin = 9;     // Pin de pasos del motor
const int enablePin = 10;  // Pin de enable del driver
const int ledVerde = A1;   // LED verde (tensión OK)
const int ledBlanco = A1;  // LED blanco (motor funcionando)
const int pulsador = A2;   // Pulsador para activar el motor
const int endstop = 7;     // Endstop para detener el motor

// Crear objeto AccelStepper
AccelStepper motor(AccelStepper::DRIVER, stepPin, dirPin);

// Variables de estado
bool motorActivo = false;
unsigned long tiempoParpadeo = 0;             // Para controlar el parpadeo del LED blanco
const unsigned long intervaloParpadeo = 500;  // Intervalo de parpadeo (500ms)

// Pines para la pantalla TFT
#define TFT_CS A5       // Pin Chip Select
#define TFT_RST A4      // Pin Reset
#define TFT_DC A3       // Pin Data/Command
#define TFT_MOSI 11     // Pin MOSI para SPI
#define TFT_SCK 13      // Pin Clock para SPI
#define TFT_LED A0      // Pin para el backlight de la pantalla
#define TFT_MISO 12     // No usado para TFT
#define GFX_BL TFT_LED  // Pin para controlar el backlight

// Pines de conexión del controlador táctil XPT2046
#define T_CS    2    // Pin Chip Select del controlador táctil
#define T_IRQ   6    // Pin de interrupción del controlador táctil (opcional)

// Velocidad del SPI (reducida para mejor estabilidad)
#define SPI_SPEED 20000000  // 40 MHz (puedes probar valores más bajos si sigue habiendo problemas)

// Crear una instancia de la pantalla
// Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST, TFT_MISO);
// XPT2046_Touchscreen ts(T_CS, -1);  // No usar interrupciones para facilitar la calibración

void setup() {
  // Configuración de los pines del motor
  delay(500);
  //setCpuFrequencyMhz(240);

  pinMode(TFT_LED, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledBlanco, OUTPUT);
  pinMode(pulsador, INPUT_PULLUP);
  pinMode(endstop, INPUT_PULLUP);
  pinMode(enablePin, OUTPUT);  // Pin Enable del driver
  pinMode(dirPin, OUTPUT);     // Configurar dirPin como salida
  pinMode(stepPin, OUTPUT);    // Configurar stepPin como salida

  digitalWrite(TFT_LED, HIGH);

  // Inicializar la pantalla táctil

  // Encender el LED verde indicando que hay tensión
  digitalWrite(ledVerde, HIGH);

  // Desactivar el motor al inicio
  digitalWrite(enablePin, HIGH);  // Desactivar el driver (Enable es activo en LOW)

  // Configurar aceleración y velocidad del motor
  motor.setMaxSpeed(5000);     // Establecer velocidad máxima del motor
  motor.setAcceleration(200);  // Establecer aceleración del motor

  // Iniciar comunicación serie
  delay(800);
  Serial.begin(9600);
  Serial.println("Iniciando ILI9341...");

  // Añadir un pequeño retraso antes de inicializar la pantalla

  // Inicializar la pantalla con una velocidad SPI ajustada
  tft.begin(SPI_SPEED);

  // Añadir otro retraso después de la inicialización
  delay(500);

  // Establecer la rotación de la pantalla (opcional, 0 a 3)
  tft.setRotation(3);
  delay(500);
  // Rellenar la pantalla de negro
  tft.fillScreen(ILI9341_BLACK);

  // Dibujar un rectángulo de prueba
  // tft.fillRect(10, 10, 100, 50, ILI9341_RED);  // Un rectángulo rojo en la esquina superior izquierda
  delay(500);
  // Establecer el color del texto y el fondo
  tft.setTextColor(ILI9341_WHITE);  // Texto blanco
  tft.setTextSize(5);               // Tamaño de texto
  tft.setCursor(25, 100);
  tft.println("RESPOOLER");

  delay(500);
  tft.setTextColor(ILI9341_WHITE);  // Texto blanco
  tft.setTextSize(3);               // Tamaño de texto
  tft.setCursor(95, 160);
  tft.println("By Quim");
  delay(500);

  Serial.println("Prueba completada.");
}

void loop() {
  // Control del motor y LEDs
  if (digitalRead(pulsador) == LOW && digitalRead(endstop) == HIGH) {
    if (!motorActivo) {
      motorActivo = true;
      digitalWrite(enablePin, LOW);  // Activar el driver (Enable es activo en LOW)
      motor.setSpeed(-4000);         // Velocidad continua (ajusta según necesidad)
      digitalWrite(ledBlanco, LOW);  // Apagar LED blanco momentáneamente (inicia el parpadeo)
    }
    motor.runSpeed();      // El motor gira indefinidamente
    parpadearLedBlanco();  // El LED blanco parpadea mientras el motor funciona
    
  } else {
    if (motorActivo) {
      motorActivo = false;
      motor.stop();                   // Detener el motor
      digitalWrite(enablePin, HIGH);  // Desactivar el driver (Enable es activo en LOW)
      digitalWrite(ledBlanco, HIGH);  // LED blanco fijo indicando que el motor se ha detenido
    }
  }
}

// Función para parpadear el LED blanco
void parpadearLedBlanco() {
  unsigned long tiempoActual = millis();
  if (tiempoActual - tiempoParpadeo >= intervaloParpadeo) {
    tiempoParpadeo = tiempoActual;
    // Cambiar el estado del LED para hacer el efecto de parpadeo
    digitalWrite(ledBlanco, !digitalRead(ledBlanco));
  }
}
