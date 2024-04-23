#define x_paso1 34     // Define el Pin de STEP para Motor de eje X
#define x_dire1 35     // Define el Pin de DIR  para Motor de eje X
#define enablePin1 32  // Define el Pin de EN para el DRV correspondiente

#define x_paso2 27     // Define el Pin de STEP para Motor de eje X
#define x_dire2 14     // Define el Pin de DIR  para Motor de eje X
#define enablePin2 23  // Define el Pin de EN para el DRV correspondiente

int retardoInicial = 2000;  // Menor numero el giro es mas rapido
int retardoFinal = 800;
int steps = 1000;  // durante cuanto timpo da el giro el motor (vueltas)

void setup() {
  // Serial.begin(9600);
  pinMode(x_paso1, OUTPUT);
  pinMode(x_paso2, OUTPUT);
  pinMode(x_dire1, OUTPUT);
  pinMode(x_dire2, OUTPUT);
  pinMode(enablePin1, OUTPUT);
  digitalWrite(enablePin1, HIGH);
  digitalWrite(x_paso1, HIGH);
  digitalWrite(x_dire1, HIGH);
  digitalWrite(enablePin2, HIGH);
  digitalWrite(x_paso2, HIGH);
  digitalWrite(x_dire2, HIGH);

  delay(100000);
}

void loop() {
  giro(x_paso1, x_paso2, x_dire1, 1, steps);
  delay(500);
  giro(x_paso1, x_paso2, x_dire1, 0, steps);
  delay(500);
}


void giro(int paso1, int paso2, int dire_, int dir, int _steps) {

  digitalWrite(dire_, dir);  // direccion de giro 0
  digitalWrite(dire_, dir);  // direccion de giro 0
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
    delayMicroseconds(retardo);
    digitalWrite(paso2, LOW);
    delayMicroseconds(retardo);

    digitalWrite(paso1, HIGH);
    delayMicroseconds(retardo);
    digitalWrite(paso1, LOW);
    delayMicroseconds(retardo);
  }
}
