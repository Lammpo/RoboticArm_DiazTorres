// #include "FastAccelStepper.h"
#include "Hardware_settings.h"
#include "Wire.h"
#include "Adafruit_INA219.h"


Adafruit_INA219 ina219;

// FastAccelStepperEngine engine = FastAccelStepperEngine();

// FastAccelStepper *stepperDRV1 = NULL;
// FastAccelStepper *stepperDRV2 = NULL;
const int velBrazo = 6000;
const int velGiro = 1000;
const int acel = 1000;
bool dir = false;


//INA219
float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float power_mW = 0;
int counter = 0;
void setup() {
  Serial.begin(115200);
  // engine.init();
  // stepperDRV1 = engine.stepperConnectToPin(STEP_PIN_DRV1);
  // stepperDRV2 = engine.stepperConnectToPin(STEP_PIN_DRV2);
  if (!ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
  }
  pinMode(ENABLE_PIN_DRV1, OUTPUT);
  pinMode(STEP_PIN_DRV1, OUTPUT);
  pinMode(DIR_PIN_DRV1, OUTPUT);

  pinMode(ENABLE_PIN_DRV2, OUTPUT);
  pinMode(STEP_PIN_DRV2, OUTPUT);
  pinMode(DIR_PIN_DRV2, OUTPUT);

  pinMode(ENABLE_PIN_DRV3, OUTPUT);
  pinMode(STEP_PIN_DRV3, OUTPUT);
  pinMode(DIR_PIN_DRV3, OUTPUT);

  pinMode(ENABLE_PIN_DRV4, OUTPUT);
  pinMode(STEP_PIN_DRV4, OUTPUT);
  pinMode(DIR_PIN_DRV4, OUTPUT);

  // Serial.print("Iniciando");
  // for(int i=0; i<5;i++){
  //   delay(300);
  //   Serial.print(".");
  // }
  delay(500);
  digitalWrite(ENABLE_PIN_DRV1, HIGH);
  digitalWrite(ENABLE_PIN_DRV2, HIGH);
  digitalWrite(ENABLE_PIN_DRV3, HIGH);
  digitalWrite(ENABLE_PIN_DRV4, HIGH);
  delay(500);
  digitalWrite(ENABLE_PIN_DRV1, LOW);
  digitalWrite(ENABLE_PIN_DRV2, LOW);
  digitalWrite(ENABLE_PIN_DRV3, LOW);
  digitalWrite(ENABLE_PIN_DRV4, LOW);

  // if (stepperDRV1 && stepperDRV2) {
  //   Serial.print("Hemos entrado");

  //   stepperDRV1->setDirectionPin(DIR_PIN_DRV1);
  //   stepperDRV2->setDirectionPin(DIR_PIN_DRV2);

  //
  //   stepperDRV2->setSpeedInHz(velBrazo);  // the parameter is steps/s !!!
  //   stepperDRV1->setSpeedInHz(velGiro);    // the parameter is steps/s !!!

  //   stepperDRV2->setAcceleration(acel);
  //   stepperDRV1->setAcceleration(acel);
  // }
  Serial.print("BV");
  Serial.print("\t");  // Bus Voltage
  Serial.print("SV");
  Serial.print("\t");  // Shunt Voltage
  Serial.print("LV");
  Serial.print("\t");  // Load Voltage
  Serial.print("C");
  Serial.print("\t");   // Current
  Serial.println("P");  // Power
  delay(2000);
  //home();
  //TestMovement();
  //testingHardcode();
}
// int dirGiro = -1;
// int dirBrazo = -1;
// int stepsGiro = 1000;
// int stepsBrazo = 6000;

// void home(FastAccelStepper *stepper) {
//   const int homeVel = 1000;
//   const int homeAcel = 1000;
//   const int homePrecitionVel = 50;
//   const int homePrecitionAcel = 1000;
//   const bool first_aproach_finished = false;

//   Serial.println("Comenzando movimiento");
//   delay(500);
//   stepper->setSpeedInHz(homeVel);  // the parameter is steps/s !!!
//   stepper->setAcceleration(homeAcel);
//   stepper->move(-2000);  //Será movimiento continuo realmente

//   //Cuando ocurra la interrupción comienza la segunda fase
//   while (stepper->isRunning())
//     ;
//   if (!stepper->isRunning()) {
//     Serial.println("Buscando desconectar el fin de carrera");
//     delay(1000);
//     stepper->setSpeedInHz(homePrecitionVel);  // the parameter is steps/s !!!
//     stepper->setAcceleration(homePrecitionAcel);
//     stepper->move(500);
//   }
// }

// void TestMovement() {
// home(stepperDRV1);
// }

void loop() {



  
  dir = !dir;
  digitalWrite(DIR_PIN_DRV1, dir);
  digitalWrite(DIR_PIN_DRV2, dir);
  digitalWrite(DIR_PIN_DRV3, dir);
  digitalWrite(DIR_PIN_DRV4, dir);
  delay(500);
  testingHardCode();
  //Serial.println("hola");
}


// int revToStep(float rev, float reduction = 1, int step_per_rev = 200) {
//   int steps = rev * step_per_rev * reduction;
//   Serial.print("steps:");
//   Serial.println(steps);
//   return steps;
// }

// void measuring(){
//   shuntvoltage += ina219.getShuntVoltage_mV();
//   busvoltage += ina219.getBusVoltage_V();
//   current_mA += ina219.getCurrent_mA();
//   power_mW += ina219.getPower_mW();
//   loadvoltage += busvoltage + (shuntvoltage / 1000);
//   counter++;
//   if (counter == 10) {
//     counter = 0;
//     Serial.print(busvoltage / 10);
//     Serial.print("\t");
//     Serial.print(shuntvoltage / 10);
//     Serial.print("\t");
//     Serial.print(loadvoltage / 10);
//     Serial.print("\t");
//     Serial.print(current_mA / 10);
//     Serial.print("\t");
//     Serial.println(power_mW / 10);

//     shuntvoltage = 0;
//     busvoltage = 0;
//     current_mA = 0;
//     power_mW = 0;
//     loadvoltage = 0;
//   }
// }

void allTogether(bool active) {
  digitalWrite(STEP_PIN_DRV1, active);
  digitalWrite(STEP_PIN_DRV2, active);
  digitalWrite(STEP_PIN_DRV3, active);
  digitalWrite(STEP_PIN_DRV4, active);
}
void testingHardCode() {
  int maxSteps = 15000;
  int maxAccelSteps = 30;


  for (int i = 1; i < maxAccelSteps; i++) {
    allTogether(true);
    delay(vel_2_msDelay(i));
    allTogether(false);
    delay(vel_2_msDelay(i));
  }

  for (int i = 1; i < maxSteps; i++) {
    // if(i%10 == 0){
    //   measuring();
    // }
    allTogether(true);
    delayMicroseconds(800);
    allTogether(false);
    delayMicroseconds(800);
  }

  for (int i = maxAccelSteps; i > 0; i--) {
    allTogether(true);
    delay(vel_2_msDelay(i));
    allTogether(false);
    delay(vel_2_msDelay(i));
  }
}


int vel_2_msDelay(int rev_per_second) {
  int delay = 10000.0 / (STEPS_PER_REV * rev_per_second + 1);
  //Serial.println(delay);
  return delay;
}

// void testing() {

//   Serial.println("Testing");
//   delay(1000);
//     int speed = 0;
//   //speed = steps_per_second(20, 50, 8, MAX_SPEED,200);
//   const int homeVel = 1000;
//   const int homeAcel = 1000;
//   stepperDRV2->setSpeedInHz(homeVel);  // the parameter is steps/s !!!
//   stepperDRV2->setAcceleration(homeAcel);
//   while(true){
//     stepperDRV2->move(-1000); //Será movimiento continuo realmente
//  while (stepperDRV2->isRunning()){}

//     delay(500);
//     stepperDRV2->move(1000); //Será movimiento continuo realmente
//   }
