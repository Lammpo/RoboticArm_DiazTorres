//      ***********************************************************************
//      *                            Example  5:                              *
//      * this example shows how run the flexy stepper in a non blocking way  *
//      * without the need of any needed function calls in the loop function  *
//      * A separate task is started to take care of sending the step signals *
//      *                                                                     *
//      *               Paul Kerspe               8.6.2020                    *
//      *                                                                     *
//      ***********************************************************************

// In order to run the example, change the IO pin numbers to match your configuration (lines 21-24)
// you might also want to change the speed and distance settings according to your setup if needed (lines 27-30)
//
// Documentation for this library can be found at:
//    https://github.com/pkerspe/ESP-FlexyStepper/blob/master/README.md
//

#include <ESP_FlexyStepper.h>

// IO pin assignments
const int MOTOR1_STEP_PIN = 26;
const int MOTOR1_DIRECTION_PIN = 25;
const int MOTOR1_ENABLE_PIN = 32;

// Speed settings
const int DISTANCE_TO_TRAVEL_IN_STEPS = 15000;
const int SPEED_IN_STEPS_PER_SECOND = 2000;
const int ACCELERATION_IN_STEPS_PER_SECOND = 200;
const int DECELERATION_IN_STEPS_PER_SECOND = 200;

// create the stepper motor object
ESP_FlexyStepper stepper1;

int previousDirection1 = -1;

String initSteppersValidator(int num_motores, int *step_pins, int *direction_pins, int *enable_pins) {
  String msg = "";
  if (sizeof(step_pins) / 4.0 != num_motores)
    msg = "Número de pines step incorrecto. ";
  if (sizeof(direction_pins) / 4.0 != num_motores)
    msg = msg + "Número de pines dir incorrecto. ";
  if (sizeof(enable_pins) / 4.0 != num_motores)
    msg = msg + "Número de pines enable incorrecto";

  return msg;
}
 ESP_FlexyStepper *steppers_init(int num_motores,int *step_pins, int *direction_pins, int *enable_pins ){
  ESP_FlexyStepper steppers[num_motores];
  msg = initSteppersValidator(num_motores, step_pins, direction_pins, enable_pins)
  if(msg!="")
  {
    Serial.print(msg);
    return;
  }
  return steppers
 }

 steppers = steppers_init
void setup()
{
  Serial.begin(115200);

  // connect and configure the stepper motor to its IO pins
  stepper1.connectToPins(MOTOR1_STEP_PIN, MOTOR1_DIRECTION_PIN);
  // set the speed and acceleration rates for the stepper motor
  stepper1.setSpeedInStepsPerSecond(SPEED_IN_STEPS_PER_SECOND);
  stepper1.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_IN_STEPS_PER_SECOND);
  stepper1.setDecelerationInStepsPerSecondPerSecond(DECELERATION_IN_STEPS_PER_SECOND);
  
  // Not start the stepper instance as a service in the "background" as a separate task
  // and the OS of the ESP will take care of invoking the processMovement() task regularily so you can do whatever you want in the loop function
  stepper1.startAsService();
  pinMode( MOTOR1_ENABLE_PIN, OUTPUT);
  digitalWrite(MOTOR1_ENABLE_PIN, LOW);
  delay(1000);

}

void loop()
{
  // just move the stepper back and forth in an endless loop

  if (stepper1.getDistanceToTargetSigned() == 0)
  {
    previousDirection1 *= -1;
    long relativeTargetPosition = DISTANCE_TO_TRAVEL_IN_STEPS * previousDirection1;
    Serial.printf("Moving stepper 1 by %ld steps\n", relativeTargetPosition);
    stepper1.setTargetPositionRelativeInSteps(relativeTargetPosition);
  }
  
  // Notice that you can now do whatever you want in the loop function without the need to call processMovement().
  // also you do not have to care if your loop processing times are too long. 
}
