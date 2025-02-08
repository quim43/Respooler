#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "XPT2046_Touchscreen.h"

// Pines de conexión de la pantalla ILI9341
#define TFT_CS   A5
#define TFT_DC   A3
#define TFT_RST  A4
#define LED A0

// Pines de conexión del controlador táctil XPT2046
#define T_CS    2    // Pin Chip Select del controlador táctil
#define T_IRQ   6    // Pin de interrupción del controlador táctil (opcional)

// Crear una instancia para la pantalla TFT y la pantalla táctil
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen ts(T_CS, -1);  // No usar interrupciones para facilitar la calibración

// Variables para almacenar los límites de la calibración
int16_t minX = 0, minY = 0, maxX = 4095, maxY = 4095;

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  // Inicializar la pantalla TFT
  tft.begin();
  tft.setRotation(3);  // Ajusta la rotación si es necesario
  tft.fillScreen(ILI9341_WHITE);  // Llenar la pantalla de blanco

  // Inicializar la pantalla táctil
  if (ts.begin()) {
    Serial.println("Pantalla táctil inicializada correctamente.");
  } else {
    Serial.println("Error al inicializar la pantalla táctil.");
  }

  calibrateTouchScreen();  // Ejecutar el proceso de calibración
}

void loop() {
  // Verificar si la pantalla táctil está siendo tocada
  if (ts.touched()) {
    TS_Point p = ts.getPoint();  // Obtener las coordenadas del toque

    // Mapear las coordenadas al tamaño de la pantalla TFT (240x320)
    int x_mapped = map(p.x, minX, maxX, 0, 330);
    int y_mapped = map(p.y, minY, maxY, 0, 230);

    // Limpiar el área de visualización de coordenadas
    tft.fillRect(0, 0, 240, 40, ILI9341_WHITE);  // Borrar el área superior

    // Mostrar las coordenadas en la pantalla
    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.print("X: "); tft.print(x_mapped);
    tft.print("  Y: "); tft.println(y_mapped);

    // También imprimir las coordenadas en el monitor serie
    Serial.print("Coordenada X: "); Serial.println(x_mapped);
    Serial.print("Coordenada Y: "); Serial.println(y_mapped);

    // Dibujar un pequeño círculo en el lugar donde tocaste
    tft.fillCircle(x_mapped, y_mapped, 5, ILI9341_RED);  // Círculo rojo de 5 píxeles de radio

    delay(200);  // Evitar múltiples detecciones rápidas
  }
}

void calibrateTouchScreen() {
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(2);

  // Punto superior izquierdo (ajustado al área visible)
  Serial.println("Calibrando el punto superior izquierdo...");
  tft.fillCircle(30, 30, 5, ILI9341_BLUE);  // Más cerca del borde pero dentro del área visible
  tft.setCursor(40, 20);
  tft.setTextSize(1);
  tft.print("Toca el punto");
  waitForTouch();
  TS_Point p1 = ts.getPoint();
  minX = p1.x;
  minY = p1.y;
  tft.fillScreen(ILI9341_WHITE);  // Limpia la pantalla después de cada punto
  delay(1000);  // Espera 1 segundo antes de mostrar el siguiente punto

  // Punto inferior derecho (ajustado al área visible)
  Serial.println("Calibrando el punto inferior derecho...");
  tft.fillCircle(290, 210, 5, ILI9341_GREEN);  // Cerca del borde pero dentro del área visible
  tft.setCursor(40, 20);
  tft.print("Toca el punto");
  waitForTouch();
  TS_Point p2 = ts.getPoint();
  maxX = p2.x;
  maxY = p2.y;
  tft.fillScreen(ILI9341_WHITE);  // Limpia la pantalla después de cada punto
  delay(1000);  // Espera 1 segundo para confirmar la calibración

  // Mostrar los valores de calibración en el monitor serie
  Serial.println("Calibración completada:");
  Serial.print("minX = "); Serial.println(minX);
  Serial.print("minY = "); Serial.println(minY);
  Serial.print("maxX = "); Serial.println(maxX);
  Serial.print("maxY = "); Serial.println(maxY);

  // Mensaje de calibración completa
  tft.setCursor(10, 10);
  tft.print("Calibracion completa!");
  delay(2000);  // Esperar 2 segundos
}

void waitForTouch() {
  while (!ts.touched()) {
    delay(10);  // Espera hasta que haya un toque
  }
  delay(200);  // Evita la detección múltiple
}
