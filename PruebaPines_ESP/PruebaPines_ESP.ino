
#include <Arduino.h>

int allPines[] = {23, 22, 1, 3, 21, 19, 18, 5, 17, 16, 4, 0, 2, 15, 8, 7, 6, 36, 39, 34, 35, 32, 33, 25, 26, 27, 14, 12, 13, 9, 10};
int numberOfPins = sizeof(allPines) / sizeof(allPines[0]);

const int LED_BUILTIN = 2;
// the setup function runs once when you press reset or power the board
void setup()
{
  // initialize all digital pin as output .
  for (int i = 0; i < numberOfPins; i++)
    pinMode(allPines[i], OUTPUT);

  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop()
{
  for (int i = 0; i < numberOfPins; i++)
  {
    pruebaHighLow(allPines[i]);
    delay(1000);
    pruebaPWM(allPines[i], 7, 50000);
  }
}

//
void pruebaPWM(pin, seg, freq)
{
  Serial.print("Probando PWM del pin: ");
  Serial.print(pin);
  Serial.print(" con frecuencia: ");
  Serial.println(freq);
  float numero_pulsos = freq * seg;
  bool state = true;
  for (float i = 0; i < numero_pulsos, i++)
  {
    digitalWrite(pin, state);
    state = !state;
  }
}

void pruebaHighLow(pin)
{
  Serial.print("Probando High-Low del pin: ");
  Serial.println(pin);
  digitalWrite(pin, LOW);
  delay(1000);
  digitalWrite(pin, HIGH);
  delay(3000);
  digitalWrite(pin, LOW)
  Serial.println("Prueba finalizada");
}
