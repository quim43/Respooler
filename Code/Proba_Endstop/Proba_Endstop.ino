// Definir pines
const int pinSensor = 7;    // Pin donde está conectado el final de carrera
const int ledPin = 13;      // Pin del LED integrado (LED_BUILTIN en el pin 13)

void setup() {
  // Configurar el pin del sensor como entrada con resistencia pull-up interna
  pinMode(pinSensor, INPUT_PULLUP);
  
  // Configurar el pin del LED como salida
  pinMode(ledPin, OUTPUT);
  
  // Inicialmente apagar el LED
  digitalWrite(ledPin, LOW);
}

void loop() {
  // Leer el estado del sensor final de carrera
  int estadoSensor = digitalRead(pinSensor);
  
  // Si el sensor se activa (el pin se pone en LOW)
  if (estadoSensor == LOW) {
    // Encender el LED
    digitalWrite(ledPin, HIGH);
  } else {
    // Apagar el LED
    digitalWrite(ledPin, LOW);
  }
}
