#include "FastAccelStepper.h"
#include "Hardware_settings.h"

// As in StepperDemo for Motor 1 on AVR
//#define dirPinStepper    5
//#define enablePinStepper 6
//#define stepPinStepper   9  // OC1A in case of AVR




FastAccelStepperEngine engine = FastAccelStepperEngine();

FastAccelStepper *stepperGiro = NULL;
FastAccelStepper *stepperBrazo = NULL;
const int velBrazo = 1500;
const int velGiro = 200;
const int acel = 1000;

void setup() {
  Serial.begin(115200);
  engine.init();
  stepperGiro = engine.stepperConnectToPin(stepPinstepperGiro);
  stepperBrazo = engine.stepperConnectToPin(stepPinstepperBrazo);

  pinMode(enablePinstepperGiro, OUTPUT);
  pinMode(enablePinstepperBrazo, OUTPUT);

  digitalWrite(enablePinstepperGiro, HIGH);
  digitalWrite(enablePinstepperBrazo, HIGH);

  if (stepperGiro && stepperBrazo) {
    Serial.print("Hemos entrado");

    stepperGiro->setDirectionPin(dirPinstepperGiro);
    stepperBrazo->setDirectionPin(dirPinstepperBrazo);

    digitalWrite(enablePinstepperBrazo, LOW);
    digitalWrite(enablePinstepperGiro, LOW);

    stepperBrazo->setSpeedInHz(velBrazo);  // the parameter is steps/s !!!
    stepperGiro->setSpeedInHz(velGiro);    // the parameter is steps/s !!!

    stepperBrazo->setAcceleration(acel);
    stepperGiro->setAcceleration(acel);
  }

  delay(2000);
  homeBrazo();
}
int dirGiro = -1;
int dirBrazo = -1;
int stepsGiro = 1000;
int stepsBrazo = 6000;
void homeBrazo() {
  const int homeVel = 200;
  const int homeAcel = 1000;
  const int homePrecitionVel = 20;
  const int homePrecitionAcel = 1000;
  const bool first_aproach_finished = false;

  Serial.println("Buscando el FC");
  stepperBrazo->setSpeedInHz(homeVel);  // the parameter is steps/s !!!
  stepperBrazo->setAcceleration(homeAcel);
  stepperBrazo->move(-1000); //Será movimiento continuo realmente

//Cuando ocurra la interrupción comienza la segunda fase
while(stepperBrazo->isRunning());
    if (!stepperBrazo->isRunning()) {
      Serial.println("Buscando desconectar el fin de carrera");
      delay(1000);
    stepperBrazo->setSpeedInHz(homePrecitionVel);  // the parameter is steps/s !!!
    stepperBrazo->setAcceleration(homePrecitionAcel);
    stepperBrazo->move(200);
  }
}
void loop() {
delay(100);
}


int revToStep(float rev, float reduction = 1, int step_per_rev = 200) {
  int steps = rev * step_per_rev * reduction;
  Serial.print("steps:");
  Serial.println(steps);
  return steps;
}