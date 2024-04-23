const int motorChannel = 0;
const int motorPin = 12;
const int frequency = 100000; //100 KHz los permite el ESP32 y el ULN2803APG y responde perfectamente
const int resolutionADC = 8; //Que son 2^8 = 256

void setup() {
  ledcSetup(motorChannel, frequency, resolutionADC);
  ledcAttachPin(motorPin, motorChannel);
}

void loop() {
  int deltaTime = 100;
  delay(2000);
  //Aumentamos el dutyCycle desde 0 hasta 255 (12V)
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
    ledcWrite(motorChannel, dutyCycle);
    delay(deltaTime);
  }
  delay(2000);
  //Reducimos el dutyCycle hasta 0 restando tensión a las bornas del motor
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
    ledcWrite(motorChannel, dutyCycle);
    delay(deltaTime);
  }
}