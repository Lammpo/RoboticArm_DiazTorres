#define x_paso 14  // Define el Pin de STEP para Motor de eje X
#define x_dire 27  // Define el Pin de DIR  para Motor de eje X


int retardo = 1000;  // Menor numero el giro es mas rapido
int steps = 350;    // durante cuanto timpo da el giro el motor (vueltas)

void setup() {
  pinMode(x_paso, OUTPUT);
  pinMode(x_dire, OUTPUT);
}

void loop() {
  giro(x_paso, x_dire, 1, steps);
  delay(500);
  giro(x_paso, x_dire, 0, steps);
  delay(500);
}

void giro(int paso_, int dire_, int dir, int _steps) {

  if (dir == 0) {
    digitalWrite(dire_, LOW);           // direccion de giro 0
    for (int i = 0; i < _steps; i++) {  // da  pasos por un tiempo
      digitalWrite(paso_, HIGH);
      delayMicroseconds(retardo);
      digitalWrite(paso_, LOW);
      delayMicroseconds(retardo);
    }
  }
  if (dir == 1) {
    digitalWrite(dire_, HIGH);          // direccion de giro 1
    for (int i = 0; i < _steps; i++) {  // da  pasos por un tiempo
      digitalWrite(paso_, HIGH);
      delayMicroseconds(retardo);
      digitalWrite(paso_, LOW);
      delayMicroseconds(retardo);
    }
  }
}
