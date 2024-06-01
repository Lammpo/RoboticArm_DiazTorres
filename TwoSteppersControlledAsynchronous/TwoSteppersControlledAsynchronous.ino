//      ***********************************************************************
//      *                            Example  7:                              *
//      * this example shows how run the flexy stepper in a non blocking way  *
//      * without the need of any needed function calls in the loop function  *
//      * two instances are created and both are started as service           *
//      * A separate task is started to take care of sending the step signals *
//      * for each stepper instance                                           *
//      *                                                                     *
//      *               Paul Kerspe               21.1.2023                   *
//      *                                                                     *
//      ***********************************************************************

// In order to run the example, change the IO pin numbers to match your configuration (lines 21-24)
// you might also want to change the speed and distance settings according to your setup if needed (lines 23-27)
//
// Documentation for this library can be found at:
//    https://github.com/pkerspe/ESP-FlexyStepper/blob/master/README.md
//

#include <ESP_FlexyStepper.h>

// IO pin assignments
const int MOTOR_1_STEP_PIN = 26;
const int MOTOR_1_DIRECTION_PIN = 25;
const int MOTOR_1_ENABLE = 32;

const int MOTOR_2_STEP_PIN = 27;
const int MOTOR_2_DIRECTION_PIN = 14;
const int MOTOR_2_ENABLE = 33;

// Speed settings
const int DISTANCE_TO_TRAVEL_IN_STEPS = 8000;
const int SPEED_IN_STEPS_PER_SECOND = 1000;
const int ACCELERATION_IN_STEPS_PER_SECOND = 800;
const int DECELERATION_IN_STEPS_PER_SECOND = 800;

// create the stepper motor object
ESP_FlexyStepper stepper_x;
ESP_FlexyStepper stepper_y;

int previousDirection = 1;

void setup()
{
  Serial.begin(115200);
  // connect and configure the stepper motor to its IO pins
  stepper_x.connectToPins(MOTOR_1_STEP_PIN, MOTOR_1_DIRECTION_PIN);
  stepper_y.connectToPins(MOTOR_2_STEP_PIN, MOTOR_2_DIRECTION_PIN);
  // set the speed and acceleration rates for the stepper motor
  stepper_x.setSpeedInStepsPerSecond(SPEED_IN_STEPS_PER_SECOND);
  stepper_x.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_IN_STEPS_PER_SECOND);
  stepper_x.setDecelerationInStepsPerSecondPerSecond(DECELERATION_IN_STEPS_PER_SECOND);

  stepper_y.setSpeedInStepsPerSecond(SPEED_IN_STEPS_PER_SECOND);
  stepper_y.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_IN_STEPS_PER_SECOND);
  stepper_y.setDecelerationInStepsPerSecondPerSecond(DECELERATION_IN_STEPS_PER_SECOND);
  
  // Not start the stepper instance as a service in the "background" as a separate task
  // and the OS of the ESP will take care of invoking the processMovement() task regularily so you can do whatever you want in the loop function
  stepper_x.startAsService(0);
  stepper_y.startAsService(0);
  delay(2000);
  pinMode(MOTOR_1_ENABLE, OUTPUT);
  digitalWrite(MOTOR_1_ENABLE, LOW);
  pinMode(MOTOR_2_ENABLE, OUTPUT);
  digitalWrite(MOTOR_2_ENABLE, LOW);

}

void loop()
{
  // just move the stepper back and forth in an endless loop
  if (stepper_x.getDistanceToTargetSigned() == 0)
  {
    Serial.printf("Stepper positions: X: %i, Y: %i\n", stepper_x.getCurrentPositionInSteps(), stepper_y.getCurrentPositionInSteps());
    delay(2000);
    previousDirection *= -1;
    long relativeTargetPosition = DISTANCE_TO_TRAVEL_IN_STEPS * previousDirection;
    Serial.printf("Moving both stepper motors by %ld steps\n", relativeTargetPosition);
    stepper_x.setTargetPositionRelativeInSteps(relativeTargetPosition);
    stepper_y.setTargetPositionRelativeInSteps(relativeTargetPosition);
  }
  Serial.printf("Stepper positions: X: %i, Y: %i\n", stepper_x.getCurrentPositionInSteps(), stepper_y.getCurrentPositionInSteps());
  delay(100);
  
  
  // Notice that you can now do whatever you want in the loop function without the need to call processMovement().
  // also you do not have to care if your loop processing times are too long. 
}
