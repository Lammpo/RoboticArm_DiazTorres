
//#include <Arduino.h>
//PINES MAL para usarlos con pinMode(OUTPUT): 1,3, 6, 7, 8, 9, 10
//PINES solo input: 34,35,36,39
int allPinesESP[] = { 23, 22, 1, 3, 21, 19, 18, 5, 17, 16, 4, 0, 2, 15, 8, 7, 6, 36, 39, 34, 35, 32, 33, 25, 26, 27, 14, 12, 13, 9, 10 };
int allPinesOk[] = { 23, 22, 21, 19, 18, 5, 17, 16, 4, 0, 2, 15, 32, 33, 25, 26, 27, 14, 12, 13 };
int numberOfPins = sizeof(allPinesOk) / sizeof(allPinesOk[0]);

const int LED_BUILTIN = 2;
// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  Serial.println("Inicializando todos los pines como OUTPUT");
  delay(1000);
  // initialize all digital pin as output .
  for (int i = 0; i < numberOfPins; i++) {
    Serial.print("Inicializando ");
    Serial.println(allPinesOk[i]);
    pinMode(allPinesOk[i], OUTPUT);
  }
}

// the loop function runs over and over again forever
void loop() {
  for (int i = 0; i < numberOfPins; i++) {
    Serial.println("");
    int pinNumber = allPinesOk[i];
    pruebaHighLow(pinNumber);
    delay(2000);
    pruebaPWM(pinNumber, 15, 50);
  }
}

// Frecuencia en KHz
void pruebaPWM(int pin, int seg, float freq) {
  Serial.print("Probando PWM del pin: ");
  Serial.print(pin);
  Serial.print(" con frecuencia: ");
  Serial.print(freq);
  Serial.println(" KHz.");

  float numero_pulsos = freq * seg * 1000;
  bool state = true;
  int delay = 500 / freq;
  //Serial.println("Número de pulsos" + String(numero_pulsos) + " Delay: " + String(delay));
  for (float i = 0; i < numero_pulsos; i++) {
    digitalWrite(pin, state);
    state = !state;
    delayMicroseconds(delay);
  }
}

void pruebaHighLow(int pin) {
  Serial.print("Probando High-Low del pin: ");
  Serial.println(pin);
  digitalWrite(pin, LOW);
  delay(1000);
  digitalWrite(pin, HIGH);
  delay(2000);
  digitalWrite(pin, LOW);
  Serial.println("Prueba finalizada");
}
