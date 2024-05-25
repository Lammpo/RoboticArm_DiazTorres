#define step1 26       // Define el Pin de STEP para Motor de eje X
#define dire1 25       // Define el Pin de DIR  para Motor de eje X
#define enablePin1 32  // Define el Pin de EN para el DRV correspondiente

#define step2 27       // Define el Pin de STEP para Motor de eje X
#define dire2 14       // Define el Pin de DIR  para Motor de eje X
#define enablePin2 33  // Define el Pin de EN para el DRV correspondiente

int retardoInicial = 2000;  // Menor numero el giro es mas rapido
int retardoFinal = 800;
int steps = 1000;  // durante cuanto timpo da el giro el motor (vueltas)

void setup() {
  Serial.begin(9600);
  pinMode(step1, OUTPUT);
  pinMode(step2, OUTPUT);
  pinMode(dire1, OUTPUT);
  pinMode(dire2, OUTPUT);
  pinMode(enablePin1, OUTPUT);
  pinMode(enablePin2, OUTPUT);

  digitalWrite(enablePin1, LOW);
  // digitalWrite(step1, HIGH);
  // digitalWrite(dire1, HIGH);
  digitalWrite(enablePin2, LOW);
  // digitalWrite(step2, HIGH);
  // digitalWrite(dire2, HIGH);

  delay(2000);
}

void loop() {
  Serial.println("Giro para un lado");
  giro(step1, step2, dire1, dire2, 1, steps);
  delay(500);
  Serial.println("Giro para otro lado");
  giro(step1, step2, dire1, dire2, 0, steps);
  delay(500);
}


void giro(int paso1, int paso2, int pinDire1, int pinDire2, int dir, int _steps) {

  digitalWrite(pinDire1, dir);  // direccion de giro 0
  digitalWrite(pinDire2, dir);  // direccion de giro 0
  float delta = abs(retardoInicial - retardoFinal);
  // Serial.print("delta:");
  // Serial.println(delta);

  int div = _steps / 2;
  for (int i = 0; i < _steps; i++) {  // da  pasos por un tiempo
    int retardo = retardoInicial - (delta / div) * i;
    if (i > _steps / 2) {
      retardo = retardoFinal + (delta / div) * (i - _steps / 2);
    }
    // Serial.print("Retardo:");
    // Serial.println(retardo);
    digitalWrite(paso2, HIGH);
    digitalWrite(paso1, HIGH);
    delayMicroseconds(retardo);
    digitalWrite(paso2, LOW);
    digitalWrite(paso1, LOW);
    delayMicroseconds(retardo);

  }
}
