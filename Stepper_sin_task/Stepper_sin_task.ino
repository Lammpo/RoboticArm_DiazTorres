
//      ******************************************************************
//      *                                                                *
//      *         Simple example for controlling a stepper motor         *
//      *                                                                *
//      *            Paul Kerspe               4.6.2020                  *
//      *                                                                *
//      ******************************************************************

// This is the simplest example of how to run a stepper motor.  
//
// Documentation for this library can be found at:
//    https://github.com/pkerspe/ESP-FlexyStepper/blob/master/README.md
//
// This library requires that your stepper motor be connected to the ESP 
// using a driver that has a "Step and Direction" interface.  
// Examples of these are:
//
//    Pololu's DRV8825 Stepper Motor Driver Carrier:
//        https://www.pololu.com/product/2133
//
//    Pololu's A4988 Stepper Motor Driver Carrier:
//        https://www.pololu.com/product/2980
//
//    Sparkfun's Big Easy Driver:
//        https://www.sparkfun.com/products/12859
//
//    GeckoDrive G203V industrial controller:
//        https://www.geckodrive.com/g203v.html
//
// For all driver boards, it is VERY important that you set the motor 
// current before running the example.  This is typically done by adjusting
// a potentiometer on the board.  Read the driver board's documentation to 
// learn how.

#include <ESP_FlexyStepper.h>

// IO pin assignments
const int MOTOR2_STEP_PIN = 27;
const int MOTOR2_DIRECTION_PIN = 14;
const int MOTOR2_ENABLE_PIN = 33;

// IO pin assignments
const int MOTOR1_STEP_PIN = 26;
const int MOTOR1_DIRECTION_PIN = 25;
const int MOTOR1_ENABLE_PIN = 32;

// create the stepper motor object
// ESP_FlexyStepper stepper1;
ESP_FlexyStepper stepper2;

void setup() 
{
  Serial.begin(115200);
  // connect and configure the stepper motor to its IO pins
  // stepper1.connectToPins(MOTOR1_STEP_PIN, MOTOR1_DIRECTION_PIN);
  pinMode(MOTOR2_ENABLE_PIN, OUTPUT);
  digitalWrite(MOTOR2_ENABLE_PIN,LOW);
  stepper2.connectToPins(MOTOR2_STEP_PIN, MOTOR2_DIRECTION_PIN);
  delay(2000);
}

void loop() 
{
  //
  // Note 1: It is assumed that you are using a stepper motor with a 
  // 1.8 degree step angle (which is 200 steps/revolution). This is the
  // most common type of stepper.
  //
  // Note 2: It is also assumed that your stepper driver board is  
  // configured for 1x microstepping.
  //
  // It is OK if these assumptions are not correct, your motor will just
  // turn less than a full rotation when commanded to. 
  //
  // Note 3: This example uses "relative" motions.  This means that each
  // command will move the number of steps given, starting from it's 
  // current position.
  //

  // set the speed and acceleration rates for the stepper motor
  stepper2.setSpeedInStepsPerSecond(100);
  stepper2.setAccelerationInStepsPerSecondPerSecond(100);

  // set the speed and acceleration rates for the stepper motor
  // stepper1.setSpeedInStepsPerSecond(100);
  // stepper1.setAccelerationInStepsPerSecondPerSecond(100);

  // Rotate the motor in the forward direction one revolution (200 steps). 
  // This function call will not return until the motion is complete.
  // stepper1.moveRelativeInSteps(200);
  Serial.println("2000 pasos");
  stepper2.moveRelativeInSteps(2000);
  delay(1000);
  // rotate backward 1 rotation, then wait 1 second
  // stepper1.moveRelativeInSteps(-200);
  Serial.println("-2000 pasos");
  stepper2.moveRelativeInSteps(-2000);
  delay(1000);

  // This time speedup the motor, turning 10 revolutions.  Note if you
  // tell a stepper motor to go faster than it can, it just stops.
  // stepper1.setSpeedInStepsPerSecond(800);
  // stepper1.setAccelerationInStepsPerSecondPerSecond(800);
  // stepper1.moveRelativeInSteps(200 * 10);
  Serial.println("2000 pasos rápidos");
  stepper2.setSpeedInStepsPerSecond(800);
  stepper2.setAccelerationInStepsPerSecondPerSecond(800);
  stepper2.moveRelativeInSteps(200 * 10);
  delay(2000);
}
