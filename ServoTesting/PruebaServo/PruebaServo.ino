// Include the ESP32 Arduino Servo Library instead of the original Arduino Servo Library
#include <ESP32Servo.h>

Servo servo_1;  // pin 0
Servo servo_2;  // pin 2
Servo servo_3;  // pin 15

// Possible PWM GPIO pins on the ESP32: 0(used by on-board button),2,4,5(used by on-board LED),12-19,21-23,25-27,32-33
// Possible PWM GPIO pins on the ESP32-S2: 0(used by on-board button),1-17,18(used by on-board LED),19-21,26,33-42
// Possible PWM GPIO pins on the ESP32-S3: 0(used by on-board button),1-21,35-45,47,48(used by on-board LED)
// Possible PWM GPIO pins on the ESP32-C3: 0(used by on-board button),1-7,8(used by on-board LED),9-10,18-21
int servoPin3 = 15;  // GPIO pin used to connect the servo control (digital out)
int servoPin1 = 0;
int servoPin2 = 2;
// Possible ADC pins on the ESP32: 0,2,4,12-15,32-39; 34-39 are recommended for analog input
// Possible ADC pins on the ESP32-S2: 1-20 are recommended for analog input
#define MAX_POS_DEGREE 88
#define MIN_POS_DEGREE 30

float delayPerDegreeFast = 10000;
float delayPerDegreeSlow = 10000;
float microDelay = 10000;
float antes = 0;
int posDegrees = MIN_POS_DEGREE;
bool dir = true;
void setup() {
  Serial.begin(115200);
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  servo_1.setPeriodHertz(50);  // Standard 50hz servo
  servo_1.attach(servoPin1, 500, 2400);

  servo_2.setPeriodHertz(50);  // Standard 50hz servo
  servo_2.attach(servoPin2, 500, 2400);

  servo_3.setPeriodHertz(50);  // Standard 50hz servo
  servo_3.attach(servoPin3, 500, 2400);
  delay(1000);
}
float recalculoDelay() {
  if(posDegrees == 0) return 1000000;
  if(dir)return delayPerDegreeFast;
  return delayPerDegreeSlow;
}
void tick() {
  if (dir) posDegrees++;
  else posDegrees--;
  Serial.println(posDegrees);
  servo_1.write(posDegrees);
  servo_2.write(posDegrees);
  servo_3.write(posDegrees);
  if (posDegrees == MIN_POS_DEGREE || posDegrees > MAX_POS_DEGREE) dir = !dir;
}
void loop() {


  if (micros() - antes > microDelay) {
    //Reseteo del antes con el ahora del micros
    antes = micros();

    //Acción Tick que tengamos que hacer
    tick();
    
    //Recálculo del Periodo (tiempo para el siguiente tick)
    microDelay = recalculoDelay();
  }

}
