#include "SPI.h"
#include "Adafruit_GFX.h"
#include <AccelStepper.h>
#include "Adafruit_ILI9341.h"
#include "XPT2046_Touchscreen.h"

// Incluir las imágenes
#include "bambu_lab.h"
#include "sunluxl.h"
#include "sunlu.h"
#include "rpbq.h"
#include "cb.h"
#include "c.h"

#define TFT_LED        A0
const int ledVerde =   A1;
const int ledBlanco =  A1;
const int pulsador =   A2;
#define TFT_DC         A3
#define TFT_RST        A4
#define TFT_CS         A5

#define T_CS           2
const int endstop =    7;
const int dirPin =     8;
const int stepPin =    9;
const int enablePin = 10;

// Configuración de SPI y pantalla
#define SPI_SPEED 20000000

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
AccelStepper motor(AccelStepper::DRIVER, stepPin, dirPin);
XPT2046_Touchscreen ts(T_CS, -1);

// Variables de estado
bool motorActivo = false;
bool pantallaActiva = true; 
bool estadoPulsador = HIGH; // Estado actual del pulsador
bool estadoPulsadorAnterior = HIGH; // Estado anterior del pulsador

unsigned long tiempoParpadeo = 0;
const unsigned long intervaloParpadeo = 500;

String bobinaSeleccionada =   "";
String materialSeleccionado = "";

float longitudTotalFilamento = 0.0;
float densidadMaterial =       0.0;
float diametroInterno =        0.0;
float pesoFilamento =          0.0;
long pasosAcumulados =           0;

/*
###############################################
*/
void mostrarMensajeInicio() {
  pantallaActiva = true;
  longitudTotalFilamento = 0;
  pesoFilamento = 0;

  // Dibujar la imagen de inicio en la pantalla
  tft.fillScreen(ILI9341_BLACK);
  tft.drawRGBBitmap(0, 0, rpbq, 320, 240);
  // delay(3500);  // Pausa inicial para mostrar la imagen
  
  // Esperar hasta que no haya ningún toque detectado (por si queda alguno residual)
  while (ts.touched()) {
    delay(100);
  }

  // Esperar un toque para avanzar
  Serial.println("Esperando toque en la imagen...");
  while (!ts.touched()) {
    delay(100);  // Pausa breve para evitar un bucle continuo
  }

  delay(500);  // Pausa adicional para evitar toques repetidos
  cargarBobines();
}


void cargarBobines(){
  pantallaActiva = false;
  tft.fillScreen(ILI9341_BLACK);
  tft.drawRGBBitmap(0, 0, cb, 320, 240);
  while (digitalRead(pulsador) == HIGH){} // && digitalRead(endstop) == LOW 
  delay(500);
  pantallaActiva = true;
  mostrarMenuBobina();
  }

void mostrarMenuBobina() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(20, 20);
  tft.println("Selecciona una bobina:");
  tft.drawRGBBitmap(15, 80, bambu_lab, 90, 90);  // Imagen Bambu Lab
  tft.drawRGBBitmap(115, 80, sunlu, 90, 90);     // Imagen Sunlu
  tft.drawRGBBitmap(215, 80, sunluxl, 90, 90);   // Imagen Sunlu XL
  }

void manejarTouch(int x, int y) {
  if (x > 25 && x < 115 && y > 80 && y < 170) {
    bobinaSeleccionada = "Bambu Lab";
    diametroInterno = 55.0;
    seleccionarMaterial();
  } else if (x > 125 && x < 215 && y > 80 && y < 170) {
    bobinaSeleccionada = "Sunlu";
    diametroInterno = 54.4;
    seleccionarMaterial();
  } else if (x > 225 && x < 315 && y > 80 && y < 170) {
    bobinaSeleccionada = "Sunlu XL";
    diametroInterno = 7.25;
    seleccionarMaterial();
    }
  }

void seleccionarMaterial() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(20, 20);
  tft.println("Selecciona el material:");
  tft.setCursor(50, 80); tft.println("PLA");
  tft.setCursor(50, 140); tft.println("PETG");
  tft.setCursor(50, 200); tft.println("ABS");

  while (true) {
    if (ts.touched()) {
      TS_Point p = ts.getPoint();
      int x_mapped = map(p.x, 689, 3700, 0, 320);
      int y_mapped = map(p.y, 3418, 746, 0, 240);
      if (x_mapped > 50 && x_mapped < 150) {
        if (y_mapped > 80 && y_mapped < 120) { materialSeleccionado = "PLA"; densidadMaterial = 1.24; break; }
        else if (y_mapped > 140 && y_mapped < 180) { materialSeleccionado = "PETG"; densidadMaterial = 1.27; break; }
        else if (y_mapped > 200 && y_mapped < 240) { materialSeleccionado = "ABS"; densidadMaterial = 1.04; break; }
        }
      }
    }
    confirmarSeleccion();
  }

void confirmarSeleccion() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(20, 20); tft.println("Confirma seleccion:");
  tft.setCursor(20, 60); tft.print("Bobina: "); tft.println(bobinaSeleccionada);
  tft.setCursor(20, 100); tft.print("Material: "); tft.println(materialSeleccionado);

  tft.fillRect(20, 160, 100, 50, ILI9341_GREEN); tft.setCursor(25, 175); tft.setTextSize(2); tft.println("Aceptar");
  tft.fillRect(140, 160, 100, 50, ILI9341_RED); tft.setCursor(145, 175); tft.println("Cancelar");

  while (true) {
    if (ts.touched()) {
      TS_Point p = ts.getPoint();
      int x_mapped = map(p.x, 689, 3700, 0, 320);
      int y_mapped = map(p.y, 3418, 746, 0, 240);
      if (x_mapped > 20 && x_mapped < 120 && y_mapped > 160 && y_mapped < 210) { 
        verificarCargaBobina(); 
        break; 
      } else if (x_mapped > 140 && x_mapped < 240 && y_mapped > 160 && y_mapped < 210) { 
        mostrarMenuBobina(); 
        return; 
        }
      }
    }
  }

void verificarCargaBobina() {
  pantallaActiva = false;
  tft.fillScreen(ILI9341_BLACK);
  tft.drawRGBBitmap(0, 0, c, 320, 240);
  delay(1500);
  iniciarMotor();
  }

void iniciarMotor() {
  motorActivo = true;
  digitalWrite(enablePin, LOW);
  motor.setSpeed(2000);
  digitalWrite(ledBlanco, LOW);
  digitalWrite(TFT_LED, LOW);
  pantallaActiva = false;

  // Ejecuta el motor hasta que se detenga por el endstop o el pulsador
  while (motorActivo) {
    motor.runSpeed();
    pasosAcumulados++;
    actualizarCalculos();
    parpadearLedBlanco();

    if (digitalRead(endstop) == LOW || digitalRead(pulsador) == HIGH) {
      detenerMotor();
      }
    }
  }

void detenerMotor() {
  motorActivo = false;
  motor.stop();
  digitalWrite(enablePin, HIGH);
  digitalWrite(ledBlanco, HIGH);
  digitalWrite(TFT_LED, HIGH);
  mostrarResultadosFinales();
  }

void actualizarCalculos() {
  float diametroFilamento = 1.75;
  float areaSeccionFilamento = PI * pow(diametroFilamento / 2, 2);
  float diametroEngranajeMotor = 18;
  float diametroEngranajePlato = 52;
  float ratioEngranajes = diametroEngranajePlato / diametroEngranajeMotor;
  float pasosPorVuelta = 200;
  float longitudPorVueltaPlato = PI * diametroInterno;

  float vueltasTotales = pasosAcumulados / (pasosPorVuelta * ratioEngranajes);
  longitudTotalFilamento = vueltasTotales * longitudPorVueltaPlato / 1000;
  pesoFilamento = longitudTotalFilamento * areaSeccionFilamento * densidadMaterial;
  }

void mostrarResultadosFinales() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(20, 60); tft.print("Longitud Final: "); tft.print(longitudTotalFilamento); tft.println(" m");
  tft.setCursor(20, 100); tft.print("Peso Final: "); tft.print(pesoFilamento); tft.println(" g");
  tft.setCursor(20, 160); tft.println("Pulsa para reiniciar");
  while (digitalRead(pulsador) == LOW) {}
  pantallaActiva = true;
  mostrarMensajeInicio();
  }

void parpadearLedBlanco() {
  unsigned long tiempoActual = millis();
  if (tiempoActual - tiempoParpadeo >= intervaloParpadeo) {
    tiempoParpadeo = tiempoActual;
    digitalWrite(ledBlanco, !digitalRead(ledBlanco));
    }
  }

/* ---------------------------------------------
   =============================================
   ---------------------------------------------
 */
void setup() {
  delay(500);
  pinMode(TFT_LED, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledBlanco, OUTPUT);
  pinMode(pulsador, INPUT_PULLUP);
  pinMode(endstop, INPUT_PULLUP);
  pinMode(enablePin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  digitalWrite(TFT_LED, HIGH);
  digitalWrite(ledVerde, HIGH);
  digitalWrite(enablePin, HIGH);
  motor.setMaxSpeed(5000);
  motor.setAcceleration(200);
  Serial.begin(9600);
  tft.begin(SPI_SPEED);
  tft.setRotation(3);
  ts.begin();
  mostrarMensajeInicio();
  }

void loop() {
  if (pantallaActiva && ts.touched()) {
    TS_Point p = ts.getPoint();
    int x_mapped = map(p.x, 689, 3700, 0, 320);
    int y_mapped = map(p.y, 3418, 746, 0, 240);
    tft.fillCircle(x_mapped, y_mapped, 3, ILI9341_WHITE);
    manejarTouch(x_mapped, y_mapped);
    delay(100);
    tft.fillCircle(x_mapped, y_mapped, 3, ILI9341_BLACK);
    }
  }
