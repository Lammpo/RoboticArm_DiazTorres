#include "Hardware_settings.h"


const int velBrazo = 1500;
const int velGiro = 200;
const int acel = 1000;
bool dir = false;

void setup() {
  Serial.begin(115200);

  pinMode(ENABLE_PIN_DRV1, OUTPUT);
  pinMode(ENABLE_PIN_DRV2, OUTPUT);
  pinMode(STEP_PIN_DRV1, OUTPUT);
  pinMode(DIR_PIN_DRV1, OUTPUT);

  pinMode(STEP_PIN_DRV2, OUTPUT);
  pinMode(DIR_PIN_DRV2, OUTPUT);

  digitalWrite(ENABLE_PIN_DRV1, HIGH);
  digitalWrite(ENABLE_PIN_DRV2, HIGH);



  delay(2000);
  digitalWrite(ENABLE_PIN_DRV1, LOW);
  digitalWrite(ENABLE_PIN_DRV2, LOW);
  delay(50);

}
int dirGiro = -1;
int dirBrazo = -1;
int stepsGiro = 1000;
int stepsBrazo = 6000;


void loop() {
  Serial.println("Hola");
  dir = !dir;
  digitalWrite(DIR_PIN_DRV1, dir);
  digitalWrite(DIR_PIN_DRV2, dir);
  delay(500);
  testingHardCode();
}
void allTogether(bool active) {
  digitalWrite(STEP_PIN_DRV1, active);
  digitalWrite(STEP_PIN_DRV2, active);
}
void testingHardCode() {
  int maxSteps = 3000;
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
  Serial.println(delay);
  return delay;
}

int revToStep(float rev, float reduction = 1, int step_per_rev = 200) {
  int steps = rev * step_per_rev * reduction;
  Serial.print("steps:");
  Serial.println(steps);
  return steps;
}

