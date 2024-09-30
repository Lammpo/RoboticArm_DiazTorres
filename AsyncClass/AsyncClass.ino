// Include the ESP32 Arduino Servo Library instead of the original Arduino Servo Library
#include <ESP32Servo.h>
#include "AsyncTask.h"


/*Mejoras de la versió 0 de AsyncTask
  v0: Se podría usar la misma función tick para todos si se le pasa un puntero a la variable que se quiere modificar, es decir, pasarle el servo por ejemplo


*/


// Possible ADC pins on the ESP32: 0,2,4,12-15,32-39; 34-39 are recommended for analog input
// Possible ADC pins on the ESP32-S2: 1-20 are recommended for analog input
#define MAX_POS_DEGREE 88
#define MIN_POS_DEGREE 30
//Pines de FC
#define FC_PIN1 36
#define FC_PIN2 39
#define FC_PIN3 34
#define FC_PIN4 35







Servo servo_1;  // pin 0
Servo servo_2;  // pin 2
Servo servo_3;  // pin 15

int servoPin3 = 15;  // GPIO pin used to connect the servo control (digital out)
int servoPin1 = 0;
int servoPin2 = 2;

float delayPerDegreeFast = 10000;
float delayPerDegreeSlow = 15000;
float microDelay = 10000;
float antes = 0;
int posDegrees = MIN_POS_DEGREE;
bool dir = true;
bool alwaysChecked(){
  return true;
}


// Función tick que acepta argumentos
void tickServoGral(Servo &servo) {
  if (dir) posDegrees++;
  else posDegrees--;
  Serial.println(posDegrees);
  servo.write(posDegrees);
  if (posDegrees == MIN_POS_DEGREE || posDegrees > MAX_POS_DEGREE) dir = !dir;
}

// Lambda que captura servo_3 por referencia
auto tickServo = [&servo_3]() {
    tickServoGral(servo_3);
};

AsyncTask servos(delayPerDegreeFast, alwaysChecked, tickServo, recalculoDelay);
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

  pinMode(FC_PIN1, INPUT);
  pinMode(FC_PIN2, INPUT);
  pinMode(FC_PIN3, INPUT);
  pinMode(FC_PIN4, INPUT);

  delay(1000);
}
bool checkFC(int pin){
  return digitalRead(pin);
}
float recalculoDelay() {
  int closedFC = (checkFC(FC_PIN1) + checkFC(FC_PIN2) + checkFC(FC_PIN3) + checkFC(FC_PIN4))*2000;
  return delayPerDegreeSlow - closedFC;
}
// 

void loop() {


  servos.run();

}

// void tick() {
// //   if (dir) posDegrees++;
// //   else posDegrees--;
// //   //Serial.println(posDegrees);
// //   servo_1.write(posDegrees);
// //   servo_2.write(posDegrees);
// //   servo_3.write(posDegrees);
// //   if (posDegrees == MIN_POS_DEGREE || posDegrees > MAX_POS_DEGREE) dir = !dir;
// // }