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

// Valores de calibración (obtenidos del proceso de calibración)
int16_t minX = 720, minY = 3420, maxX = 3612, maxY = 658;

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
}

void loop() {
  // Verificar si la pantalla táctil está siendo tocada
  if (ts.touched()) {
    TS_Point p = ts.getPoint();  // Obtener las coordenadas del toque

    // Mapear las coordenadas al tamaño de la pantalla TFT (240x320) usando los valores de calibración
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
