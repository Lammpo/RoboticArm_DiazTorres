int voltage(Adafruit_INA219 ina219){
  static int counter = 0;
  const int maxCounter = 10;
  static float res = 0; 
  static float shuntvoltage = 0;
  static float busvoltage = 0;
  static float current_mA = 0;
  static float loadvoltage = 0;
  static float power_mW = 0;

  shuntvoltage =+ ina219.getShuntVoltage_mV();
  busvoltage =+ ina219.getBusVoltage_V();
  current_mA =+ ina219.getCurrent_mA();
  power_mW =+ ina219.getPower_mW();
  loadvoltage =+ busvoltage + (shuntvoltage / 1000);

  if(counter>=maxCounter){
    return loadvoltage/counter;
  }
  return res;
}