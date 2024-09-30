#include "FastAccelStepper.h"
#include "Hardware_settings.h"





FastAccelStepperEngine engine = FastAccelStepperEngine();

FastAccelStepper *stepperGiro = NULL;
FastAccelStepper *stepperBrazo = NULL;
const int velBrazo = 6000;
const int velGiro = 1000;
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
  //TestMovement();
}
int dirGiro = -1;
int dirBrazo = -1;
int stepsGiro = 1000;
int stepsBrazo = 6000;

void homeBrazo() {
  const int homeVel = 1000;
  const int homeAcel = 1000;
  const int homePrecitionVel = 50;
  const int homePrecitionAcel = 1000;
  const bool first_aproach_finished = false;

  Serial.println("Buscando el FC");
  stepperBrazo->setSpeedInHz(homeVel);  // the parameter is steps/s !!!
  stepperBrazo->setAcceleration(homeAcel);
  stepperBrazo->move(-2000);  //Será movimiento continuo realmente

  //Cuando ocurra la interrupción comienza la segunda fase
  while (stepperBrazo->isRunning())
    ;
  if (!stepperBrazo->isRunning()) {
    Serial.println("Buscando desconectar el fin de carrera");
    delay(1000);
    stepperBrazo->setSpeedInHz(homePrecitionVel);  // the parameter is steps/s !!!
    stepperBrazo->setAcceleration(homePrecitionAcel);
    stepperBrazo->move(500);
  }
}

// void TestMovement() {
//   int _dirGiro = -1;
//   int _dirBrazo = -1;
//   while (1) {
//     if (!stepperBrazo->isRunning()) {
//       delay(50);
//       stepperBrazo->move(_dirBrazo * 6000);
//       _dirBrazo = _dirBrazo * -1;
//     }

//     if (!stepperGiro->isRunning()) {
//       delay(50);
//       stepperGiro->move(_dirGiro * 2000);
//       _dirGiro = _dirGiro * -1;
//     }
//   }
// }

void loop() {
}


int revToStep(float rev, float reduction = 1, int step_per_rev = 200) {
  int steps = rev * step_per_rev * reduction;
  Serial.print("steps:");
  Serial.println(steps);
  return steps;
}


void testing() {

  Serial.println("Testing");
  int speed = 0;
  //speed = steps_per_second(20, 50, 8, MAX_SPEED,200);
  Serial.print(speed);
  // stepperBrazo->setSpeedInHz(homeVel);  // the parameter is steps/s !!!
  // stepperBrazo->setAcceleration(homeAcel);
  // stepperBrazo->move(-1000); //Será movimiento continuo realmente
}