#include "FastAccelStepper.h"

// As in StepperDemo for Motor 1 on AVR
//#define dirPinStepper    5
//#define enablePinStepper 6
//#define stepPinStepper   9  // OC1A in case of AVR

// As in StepperDemo for Motor 1 on ESP32
#define dirPinStepper 25
#define enablePinStepper 32
#define stepPinStepper 26

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;
int revToStep(float rev, float reduction=1, int step_per_rev = 200){
int steps = rev*step_per_rev*reduction;
Serial.print("steps:");
Serial.println(steps);
return steps;

}
void setup() {
  Serial.begin(115200);
  engine.init();
  stepper = engine.stepperConnectToPin(stepPinStepper);
  pinMode(enablePinStepper, OUTPUT);
  digitalWrite(enablePinStepper, HIGH);
  delay(1000);
  revToStep(0.5, 1 );
   revToStep(0.5, 3 );
  if (!stepper) {
    stepper->setDirectionPin(dirPinStepper);
    // stepper->setEnablePin(enablePinStepper);
    // stepper->setAutoEnable(true);

    digitalWrite(enablePinStepper, LOW);
    // If auto enable/disable need delays, just add (one or both):
    // stepper->setDelayToEnable(50);
    // stepper->setDelayToDisable(1000);

    stepper->setSpeedInUs(1000);  // the parameter is us/step !!!
    stepper->setAcceleration(100);
    stepper->move(1000);
  }
}

void loop() {}
