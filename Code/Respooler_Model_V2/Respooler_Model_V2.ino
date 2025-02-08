// Respooler Code

// Defining Variables
const int Button = 8;
const int Sensor = 7;
const int Led_L = 9;
const int Led_R = 10;
const int En = 4;
const int Stp = 3;
const int Dir = 2;

unsigned long Acceleration = 9;
unsigned long previousMillis = 0;
unsigned long motorStartTime = 0;

const int ledBrightness = 26; // 10% brightness (value between 0 and 255)
const unsigned long sensorOffDuration = 150; // Duration in milliseconds for which the sensor must be off to stop the motor

void setup() {
  // Pin Modes
  pinMode(Button, INPUT_PULLUP);
  pinMode(Sensor, INPUT_PULLUP);
  pinMode(En, OUTPUT);
  pinMode(Stp, OUTPUT);
  pinMode(Dir, OUTPUT);
  pinMode(Led_L, OUTPUT);
  pinMode(Led_R, OUTPUT);

  // Disable stepper
  digitalWrite(En, HIGH);

  // Set stepper direction (HIGH or LOW)
  digitalWrite(Dir, HIGH);

  // Blink to indicate setup completion
  setLedBrightness(Led_L, ledBrightness);
  setLedBrightness(Led_R, ledBrightness);
  delay(500);
  turnOffLeds();
}

void loop() {
  digitalWrite(En, HIGH);

  if (digitalRead(Sensor) == HIGH) {
    if (digitalRead(Button) == LOW) {
      turnOnLeds();
    } else {
      turnOffLeds();
    }
  } else {
    setLedBrightness(Led_R, ledBrightness);

    if (digitalRead(Button) == LOW) {  // Button pressed
      runStepper();
    }
  }
}

void runStepper() {
  unsigned long Speed = 100;  // Stepper speed (lower means faster), don't go lower than 70, motor might skip!
  unsigned long Interval = 750;

  motorStartTime = millis();
  digitalWrite(En, LOW);

  while (true) {
    unsigned long currentMillis = millis();

    // Check if button is pressed and more than 1 second has passed since motor start
    if (digitalRead(Button) == LOW && (currentMillis - motorStartTime) > 1000) {  
      delay(500);
      turnOffLeds();
      break;
    }

    if (Interval > (Speed)) {
      setLedBrightness(Led_L, ledBrightness);

      stepMotor(Interval);

      if (currentMillis - previousMillis > Acceleration) {
        previousMillis = currentMillis;
        Interval -= 2;
      }

      if (digitalRead(Sensor) == HIGH) {
        if (sensorCheck(Interval)) {
          turnOffLeds();
          break;
        }
      }
    } else {
      blinkLed(Led_L, 1000);

      stepMotor(Speed);

      if (digitalRead(Button) == LOW && (currentMillis - motorStartTime) > 1000) {  // Emergency stop
        delay(500);
        turnOffLeds();
        break;
      }

      if (digitalRead(Sensor) == HIGH) {
        if (sensorCheck(Speed)) {
          turnOffLeds();
          break;
        }
      }
    }
  }
}

void stepMotor(unsigned long interval) {
  digitalWrite(Stp, HIGH);
  delayMicroseconds(interval);
  digitalWrite(Stp, LOW);
  delayMicroseconds(interval);
}

bool sensorCheck(unsigned long interval) {
  unsigned long sensorHighStart = millis();
  while (millis() - sensorHighStart < sensorOffDuration) {
    digitalWrite(Stp, HIGH);
    delayMicroseconds(interval+20);
    digitalWrite(Stp, LOW);
    delayMicroseconds(interval+20);
    
    if (digitalRead(Sensor) == LOW) {
      return false;
    }
  }
  return true;
}

void setLedBrightness(int pin, int brightness) {
  analogWrite(pin, brightness);
}

void turnOnLeds() {
  setLedBrightness(Led_L, ledBrightness);
  setLedBrightness(Led_R, ledBrightness);
}

void turnOffLeds() {
  analogWrite(Led_L, 0);
  analogWrite(Led_R, 0);
}

void blinkLed(int pin, unsigned long interval) {
  static unsigned long previousMillis = 0;
  static bool ledState = LOW;

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (ledState == LOW) {
      ledState = HIGH;
      setLedBrightness(pin, ledBrightness);
    } else {
      ledState = LOW;
      analogWrite(pin, 0);
    }
  }
}
