/*
  v1: Vamos a meter incrementos paulatinos de velocidad ahsta llegar a una velocidad determinada
  v2: Esto no tiene nada que ver con la v1 porque vamos a pasar de los delays y trataremos de
  contar el tiempo entre pulsos con la función micros, a ver como se da.
    v2_2: -Vamos a remodelar un poco la función para que sea más pequeña agrupando las accciones.
          -También le hemos metido la desaceleración
          -Es MUY IMPORTANTE la continuidad del movimiento. No se pueden dar saltos de velocidad muy grandes
          -Cada vez que cambiemos de velocidad podemos utilizar esta función que siempre acelera/desacelera
          por lo que se puede utilizar para cambiar de velocidad también (REVISAR) tal vez si la reduOn es negativa.
          -También se podría crear una función específica para la variación de velocidades.
  BIMOTOR: Vamos a intentar controlar dos motores a la vez con un mismo bucle.
  Algo interesante a tener en cuenta es que la definición static en la función, que nos ahorra cálculos, no puede utilizarse
  para dos motores distintos. Es decir, si se guardan datos para la próxima llamada de función y la función la llama otro motor
  pues estamos bien de jodidos. Voy a intentar hacerlo con clases.
  Con clases las static siguen funcionando igual así que voy a hacer que las variables que quiera mantener con su valor sean
  propias de la clase sin ser static. Esto debería funcionar y que los valores asociados a cada motor se guardaran correctamente
  Eventualmente intentaremos meterle órdenes con el código

  BimotorV2: Había un fallo en el que se usaba solo Ton_ini y no Toff para nominal y decel.
    Hemos conseguido implantar los dos motores con clases

  BimotorV3: -Vamos a pasar definitivamente de dar la velocidad con Ton_ini y Toff por lo que hallamos un parámetro para
    convertir cómodamente y sin necesidad de cálculo de Cm/s a Ton_ini Toff: Cm/s = (Diametro * pi* 10^6) /((Ton_ini + Toff)* puls*3)
    -También vamos a pasar tristemente de Toff y a dejarlo fijo en 400-500.

  BimotorV4: -Tratamos de reducir al mínimo todas las operaciones de FunMotor. Incluso las sumas y restas. He reducido las operaciones dentro
    de los propios if y quitado la división flotante en la aceleración por una suma. De locos. Para regNominal una media de 28us y acelerando 35us.
    -También hemos cambiado definitivamente todo a cm y a cm_s para que sea más cómodo.
    -Las vueltas de freno ahora cambian un poco en función de la velocidad que llevamos. Además ahora frena de manera acumulativa como cuando se acelera.
    -Ahora podemos decidir fácilmente el sentido de las ruedas sin tener que usar true o false.
    -Le metemos las librería de comunicación e introducimos un poco todo para continuar en la V5

  BimotorV5: -Metemos el antes = micros() dentro de la función funmotora justo después de activarse o desactivarse la bobina. Tiene más sentido así.
    -Hemos metido toda la comunicación y recibe correctamente todos los parámetros y órdenes.
    -Debemos ahora ver como llevar a cabo las órdenes con la FunMotora que tenemos o cambiarla para PARAR, CONTINUAR, modificar la vel... 
    -La comunicación desde que el máster ordena un mensaje y lo recibe correctamente ronda los 700us lo que está realmente bien. No interfiere a más de un pulso.

  BimotorV6: -Vamos al meollo donde definiremos como cumplir las órdenes del máster.
         V6.1:-Vamos a usar la función FunMotora para todo. Cada vez que queramos parar, continuar a lo que sea reactivamos ini_marcha con los argumentos que sean
              -Ha sido un absoluto éxito. Para empezar hemos incorporado un comportamiento nuevo en el que si los cm a recorrer son menores que la suma de los cm
                de aceleración y de frenada, se reparten los cm de recorrido entre acel y frenada, no hay reg_nominal. En vez de un trapecio es un triángulo.
              -En el loop hacemos caso o no a la petición en función del estado. p.e. si estamos INMÓVIL o PARANDO no vamos a echar cuenta a una petición de PARAR.
              -A menos que estemos INMOVIL no vamos a girar ni a hacer caso a Adelante. Si queremos mod la vel usamos mod vel
              -ADELANTE  tiene la posibilidad de incluir la velocidad como argumento.
              -GIR_XXX tiene la posibilidad de incluir los grados como argumento. Mov_continuo si no.
              -Definimos una VEL_PARADA que ya veremos si la cambiaremos.
              -Hemos reformulado todas las cuentas de ini_marcha. Ahora los cm_recorrido son efectivamente los cm que se van a recorrer. No la suma con cm_acel. 
                Se ha simplificado en gran parte y en función del estado en el que estemos (sobretodo si es PARANDO) hacemos unas cuentas u otras. Ya veremos con 
                mod_vel que hacemos.
         V6.2:-Mod_vel funciona correctamente y cambia de velocidad estándo INMOVIL y en movimiento. 
                No obstante reinicia la cuenta de cm. *CORREGIR*
  BimotorV7: REDUCIR MEMORIA COMO SEA.
          7.2: hemos reducido 1170 bytes :D. 
            Para ATtiny85: 7510 bytes (91%) of program storage space. 
                           Global variables use 226 bytes(44%) of dynamic memory
          La versión 7.2 para ATtiny funciona perfectamente.
                            7266 bytes (88%) of program storage space
                            Global variables use 218 bytes (42%) of dynamic memory
          7.3 Vamos a intentar usar la función reserve para reservar memoria del string msg y que no ocupe en la SRAM
          7.4 Pasamos de los ATtiny y metemos un parámetro nuevo de vel_giro para el joystick y apds sobre todo. Es interesante que haya una vel de giro distinta 
              a la vel nominal.
          7.5: Vamos a modificar MOD_VEL e introducimos por primera vez dos parámetros divididos por '-'
  
  BimotorV8: Esto va increible.
          8.1: vamos meter un pin para indicarle al mega que queremos mandarle un mensaje de que estamos parando o INMÓVILES    
*/
// Definiciones para Tren de Potencia
#define PULS 200  //Steps o pulsos para dar una vuelta completa
//#define Di 9.66     //Diametro actual de las ruedas
//#define pi 3.14
#define Conver_Ton_cm_s 50580     // --> Esto viene de la conversión de ton y toff a cm/s: (Diametro * pi* 10^6) / (puls*3)
#define Conver_cm2steps 19.772    // --> Esto viene de la conversión de cm a vueltas del motor: 3 [vueltasmotor/vueltas de rueda] * 200[steps/vueltasmotor] / (9.66 [diámetro] * pi)
#define Conver_grados_2_cm 0.209  //  --> Esto viene de la experimentación. Suponemos que 76.45cm son 360 grados (7.3 vueltas = 90grados)

#define VEL_PARADA 15  //  --> Esto implica un ton final de 3080. VARIAR
#define acc 50
#define alpha 0.0506
#define alpha2Acc 5.058
#define no 44
#define Cno 3371
#define CnoEmer 2522  //Equivale a 20cm/s

#define ToffNominal 500
#define VEL_MAX 60
#define VEL_MIN 20
/*** Comunicación, lista de peticiones y funciones ***/
#include <Wire.h>
#define ESCLAVO 23  //Este será el esclavo 23 p.e.
#include "HEX_PETs_and_functions.h"


char prox_msg[6] = { ' ', '\0', '\0', '\0', '\0', '\0' };
int pinPedirPermiso = 5;
bool permisoActivo = false;
bool pedirPermiso = false;
estado_motor estadoAnterior = INMOVIL;
String msg;
TiposPeticion nueva_pet_tipo = CUMPLIDA;
peticiones pet;

enum marchas {
  FORWARD,
  BACKWARD,
};


/*** Definición de la clase TrenDePotencia ***/
class TrenDePotencia {

private:

  int pasos_freno;             //Los steps que se usan para frenar
  int pasos_freno_emergencia;  //Los steps que se usan para frenar de emergencia
  float desAcel;               //La agresividad del freno

  bool polarizacion;          //En todo momento debemos saber si las bobinas están polarizadas o no
  int pasosAcel;              //Son los pulsos para el recorrido en la aceleración
  int pasos_regimen_Nominal;  //Son los pulsos en los que estamos en el regimen nominal (despues de todos los pulsos de pasosAcel y sin pulsos de frenada)
  int pasos_totales;          //pasosAcel + pasos
  int Ton_ini;                // Tiempo de las bobinas en estado ON al inicio del movimiento. Se irá reduciendo
  int TonNominal;             // Tiempo de las bobinas en estado ON en reg nominal
  int Ton;

  int n;
  int n_new;
  float c;
  int n_nominal;

  //unsigned int ToffNominal;  //Tiempo de las bobinas en estado OFF. Podría ser una costante igual a 500.
  byte pinSTEP;   //Pin para dar el pulso STEP
  byte pinDIR;    //Pin para decidir el sentido de la rueda
  bool ADELANTE;  //Esto debería mantenerse constante. Por eso está en privado.
  bool ATRAS;     //No podemos definirlo como constante y luego inicializarlo en el constructor :(


public:

  long unsigned antes;
  unsigned int steps;
  unsigned int ini_steps;
  bool ini_marcha;
  enum marchas dire_actual;
  enum estado_motor estado;
  bool mov_continuo;
  bool recalculo;
  short int vmax;



  //Constructor
  TrenDePotencia(byte pin1, byte pin2, bool forward);

  //Métodos
  void estadoMotor(float recorrido_cm, enum marchas Dire);
  int rampGenerator();
};

//Constructor
TrenDePotencia ::TrenDePotencia(byte pin1, byte pin2, bool forward) {
  pinSTEP = pin1;
  pinDIR = pin2;

  steps = 0;             //Contar los pasos que se llevan
  polarizacion = false;  //Para que siempre se parta de polarizacion de las bobinas en 0 con low en pinSTEP
  antes = 0;
  //ToffNominal = 500;  //Vamos a dejar en 500us el tiempo sin activar bobinas. Ha funcionado bien
  Ton = 0;
  ADELANTE = forward;
  ATRAS = !forward;
  ini_marcha = true;
  estado = INMOVIL;

  n_new = no;
  n = no;
  c = Cno;
  recalculo = false;
  n_nominal = 0;
  vmax = 30;

  pinMode(pinSTEP, OUTPUT);
  pinMode(pinDIR, OUTPUT);
  digitalWrite(pinSTEP, LOW);
}


//Función motora principal
void TrenDePotencia ::estadoMotor(float recorrido_cm, enum marchas Dire) {
  recalculo = true;
  if (ini_marcha) {  //Operaciones que no hace falta repetir.

    ini_steps = steps; /*Guardamos los valores actuales de steps*/
    ini_marcha = false;

    /*if (ADELANTE) Serial.print("Soy Dch: ");
    else Serial.print("Soy Izq: ");*/
    //Sentido del Rover escogido
    if (Dire == FORWARD) {
      digitalWrite(pinDIR, ADELANTE);
      dire_actual = FORWARD;
    }

    if (Dire == BACKWARD) {
      digitalWrite(pinDIR, ATRAS);
      dire_actual = BACKWARD;
    }

    /* Conversiones entre centímetros a recorrer y los pulsos del motor */
    pasos_totales = recorrido_cm * Conver_cm2steps + 2;
    //Serial.print("pt:");
    //Serial.println(pasos_totales);




    if (estado == PARANDO or estado == PARANDO_EMER) return;
    else n_new = round((vmax * vmax) / alpha2Acc);  //ACELERANDO-FRENANDO O NOMINAL

    /*
    Serial.print("N_NEW1:");
    Serial.println(n_new);
    Serial.print("N:");
    Serial.println(n);*/




    //Solo hay una n para cada velocidad y acc (que es constante)

    //Con ini_marcha = true mov_continuo nunca será false a la vez que estado = FRENANDO || NOMINTAL || PARANDO
    /* Comprobamos si el recorrido total pedido es menor que los cm necesarios para la aceleración y la frenada*/
    //Solo nos interesa hacer esto si el recorrido es cerrado no cuando mov_continuo

    if ((n_new - no) * 2 > pasos_totales and !mov_continuo and estado != PARANDO) {
      n_new = pasos_totales / 2 + no;
    }
    /*
    Serial.print("Estado en ini: ");
    Serial.println(estado);
    Serial.print("N_NEW2:");
    Serial.println(n_new);
    Serial.print("N:");
    Serial.println(n);*/
    if (n > n_new) estado = FRENANDO;
    if (n < n_new) estado = ACELERANDO;
    if (n == n_new) estado = NOMINAL;
    /*Serial.print("vmax: ");
    Serial.println(vmax);*/
    /*
    Serial.print("Estado en ini2: ");
    Serial.println(estado);*/
    return;
  }
  /*
     En realidad todo depende del tiempo entre llamadas de función. Siempre va a haber una activación
     o desactivación de una bobina. Si es acel o vel nominal o desce la diferencia es el cálculo de
     back2work.
  */
  if (!polarizacion) {  //Siempre pasan estos condicionales cada vez que la función es llamada
    digitalWrite(pinSTEP, HIGH);
    polarizacion = true;
    steps++;
  } else {
    digitalWrite(pinSTEP, LOW);
    polarizacion = false;
  }
  antes = micros();  //Una vez hemos activado o desactivado una bobina empezamos a contar el tiempo

  /*** DECISIÓN DEL ESTADO MOTOR ***/

  //PARANDO va por separado da igual si mov continuo o no
  if (estado == PARANDO && n <= no) {
    estado = INMOVIL;
    n = no;
    c = Cno;
    steps = 0;
    return;
  }
  if (estado == PARANDO_EMER && c < CnoEmer) {
    estado = INMOVIL;
    n = no;
    c = Cno;
    steps = 0;
    return;
  }

  if (mov_continuo) {

    if (n > n_new) estado = FRENANDO;
    if (n < n_new) estado = ACELERANDO;
    if (n == n_new) estado = NOMINAL;
  } else if (estado != PARANDO && estado != PARANDO_EMER) {
    if (steps - ini_steps < n_new - no) estado = ACELERANDO;                    //Suponemos que si no hay mov_continuo siempre se empieza parado
    else if (steps - ini_steps < pasos_totales - n_new + no) estado = NOMINAL;  //n_new - no serán los pasos necesarios desde la vel actual hasta VEL_PARADA
    else estado = PARANDO;                                                      //En el mov no contiuo nunca se frena.
  }
}

int TrenDePotencia ::rampGenerator() {
  recalculo = false;
  if (!polarizacion) return ToffNominal;
  switch (estado) {
    case ACELERANDO:
      n++;
      c = c - 2.0 * c / (4 * n + 1);
      break;

    case NOMINAL:
      //No se hace nada y nos quedamos con la c anterior.
      n_nominal++;
      break;

    case FRENANDO:
      c = c - 2 * c / (4 * (-n) + 1);
      n--;
      break;

    case PARANDO:
      c = c - 2 * c / (4 * (-n) + 1);
      n--;
      break;
    case PARANDO_EMER:
      c = c - 2 * c / (4 * (-n + no) + 1);
      n--;
      break;
  }

  return round(c - ToffNominal);
}

unsigned int back2workIzq = 1;
unsigned int back2workDch = 1;


int vel_adel = 40;
int vel_atrs = 25;
int vel_giro = 20;


float recorrido_cm_izq = 0;
float recorrido_cm_dch = 0;

float antesCalRampDch = 0;
float antesCalRampIz = 0;

bool shut_down = true;
byte pin_shut_down = 7;



TrenDePotencia md(10, 11, false);  //pinSTEP-MARRÓN y pinDIR-AZUL
TrenDePotencia mi(9, 8, true);
bool flagIni = false;
float res2;
float res;
void setup() {
  //Serial.begin(9600);
  pinMode(pin_shut_down, OUTPUT);
  digitalWrite(pin_shut_down, shut_down);

  pinMode(pinPedirPermiso, OUTPUT);
  digitalWrite(pinPedirPermiso, LOW);

  Wire.begin(ESCLAVO);  // unirse al bus i2c con la direccion #23
  Wire.setClock(400000);
  Wire.onRequest(TeCuento);  // registra un evento para enviar datos al máster
  Wire.onReceive(Cuentame);  // registra un evento para recibir datos del máser

  msg.reserve(7);
  delay(500);
}


void loop() {

  //AQUÍ METEMOS EL MENSAJE Y SACAMOS LA INFORMACIÓN QUE NOS HAN DADO DE VELOCIDAD p.e.
  //cambiamos vel y luego en CONTINUAR leemos vel mandándosela al master.
  // SI ES UNA ORDEN SE PUEDE TOMAR EL TIEMPO EN EL LOOP PARA AVERIGUAR QUE ORDEN ES.
  int parametro_msg = 0;
  int parametro_msg2 = 0;
  byte len_msg;
  int grados_giro;
  int separador = -1;
  if (nueva_pet_tipo == ORDEN) {

    len_msg = msg.length();
    if (len_msg > 1) {
      separador = msg.indexOf("-");
      if (separador != -1) {
        parametro_msg = msg.substring(1, separador).toInt();
        parametro_msg2 = msg.substring(separador + 1, len_msg).toInt();
      } else {
        parametro_msg = msg.substring(1, len_msg).toInt();  //Solo tenemos un argumento
      }
    }
    switch (pet) {
      case PARAR:
        if (md.estado != INMOVIL and md.estado != PARANDO) {
          md.estado = PARANDO;
          md.ini_marcha = true;
        }

        if (mi.estado != INMOVIL and mi.estado != PARANDO) {
          mi.estado = PARANDO;
          mi.ini_marcha = true;
        }

        md.mov_continuo = false;
        mi.mov_continuo = false;
        break;

      case PARAR_DE_INMEDIATO:

        if (md.estado != INMOVIL) {
          md.estado = PARANDO_EMER;
          md.ini_marcha = true;
        }

        if (mi.estado != INMOVIL) {
          mi.estado = PARANDO_EMER;
          mi.ini_marcha = true;
        }
        md.mov_continuo = false;
        mi.mov_continuo = false;
        break;

      case ADELANTE:
        if (md.estado == INMOVIL and mi.estado == INMOVIL and !shut_down) {  //Solo si estamos inmoviles le hacemos caso
          mi.dire_actual = FORWARD;
          md.dire_actual = FORWARD;
          if (len_msg > 1) vel_adel = parametro_msg;  //Vemos si tenemos más información.

          md.ini_marcha = true;
          mi.ini_marcha = true;
          md.mov_continuo = true;
          mi.mov_continuo = true;
          md.estado = ACELERANDO;  // TODO: Quitar esto, con la vmax que queremos llegar y la actual lo podemos saber mejor
          mi.estado = ACELERANDO;
          md.vmax = vel_adel;
          mi.vmax = vel_adel;
        }

        break;

      case GIR_DCH:
        if (md.estado == INMOVIL and mi.estado == INMOVIL and !shut_down) {
          mi.dire_actual = FORWARD;
          md.dire_actual = BACKWARD;
          md.vmax = vel_giro;
          mi.vmax = vel_giro;
          if (len_msg > 1) {  //Vemos si tenemos más información.
            grados_giro = parametro_msg;
            recorrido_cm_izq = Conver_grados_2_cm * grados_giro;
            recorrido_cm_dch = recorrido_cm_izq;
            md.mov_continuo = false;
            mi.mov_continuo = false;
          } else {
            md.mov_continuo = true;
            mi.mov_continuo = true;
          }
          md.ini_marcha = true;
          mi.ini_marcha = true;
          md.estado = ACELERANDO;
          mi.estado = ACELERANDO;
        }
        break;

      case GIR_IZQ:
        if (md.estado == INMOVIL and mi.estado == INMOVIL and !shut_down) {  //Exactamente igual que GIR_DCH pero cambiando la dirección
          mi.dire_actual = BACKWARD;
          md.dire_actual = FORWARD;
          md.vmax = vel_giro;
          mi.vmax = vel_giro;

          if (len_msg > 1) {  //Vemos si tenemos más información.
            grados_giro = parametro_msg;
            recorrido_cm_izq = Conver_grados_2_cm * grados_giro;
            recorrido_cm_dch = recorrido_cm_izq;
            md.mov_continuo = false;
            mi.mov_continuo = false;
          } else {  //Si no tenemos los grados explicitamente entonces giramos continuamente
            md.mov_continuo = true;
            mi.mov_continuo = true;
          }
          md.ini_marcha = true;
          mi.ini_marcha = true;
          md.estado = ACELERANDO;  //Suponemos que estamos parados y vamos a acelerar
          mi.estado = ACELERANDO;  //Al cambiar esto impedimos que se vuelva a entrar aquí también
        }
        break;

      case ATRAS:
        if (md.estado == INMOVIL and mi.estado == INMOVIL and !shut_down) {  //Solo si estamos inmoviles le hacemos caso
          mi.dire_actual = BACKWARD;
          md.dire_actual = BACKWARD;

          md.ini_marcha = true;
          mi.ini_marcha = true;
          md.mov_continuo = true;
          mi.mov_continuo = true;
          md.estado = ACELERANDO;  // TODO: Quitar esto, con la vmax que queremos llegar y la actual lo podemos saber mejor
          mi.estado = ACELERANDO;
          md.vmax = vel_atrs;
          mi.vmax = vel_atrs;
        }
        break;

      case MOD_VEL_IZQ:
        if (len_msg > 1)  //Por seguridad no vaya a ser que no se haya enviado nada
        {
          if (parametro_msg < VEL_MIN) parametro_msg = VEL_MIN;  //Por seguridad
          if (parametro_msg > VEL_MAX) parametro_msg = VEL_MAX;

          mi.vmax = parametro_msg;
          if (mi.estado == NOMINAL or mi.estado == ACELERANDO)  //Solamente queremos que precalcule si se está moviendo en ese momento. Si no, simplemente se cambia la velocidad y cuando se vuelva a mover se aplicará
          {
            mi.ini_marcha = true;  //Está hecho para que se cambie solo si está en movimiento y se desacelere o acelere.
          }
        }
        break;

      case MOD_VEL_DCH:
        if (len_msg > 1)  //Por seguridad no vaya a ser que no se haya enviado nada
        {
          if (parametro_msg < VEL_MIN) parametro_msg = VEL_MIN;  //Por seguridad
          if (parametro_msg > VEL_MAX) parametro_msg = VEL_MAX;


          md.vmax = parametro_msg;
          if (md.estado == NOMINAL or md.estado == ACELERANDO)  //Solamente queremos que precalcule si se está moviendo en ese momento. Si no, simplemente se cambia la velocidad y cuando se vuelva a mover se aplicará
          {
            md.ini_marcha = true;  //Está hecho para que se cambie solo si está en movimiento y se desacelere o acelere.
          }
        }
        break;


      case MOD_VEL_GIRO:
        if (len_msg > 1)  //Por seguridad no vaya a ser que no se haya enviado nada
        {
          if (parametro_msg < VEL_MIN) parametro_msg = VEL_MIN;  //Por seguridad
          if (parametro_msg > VEL_MAX) parametro_msg = VEL_MAX;
          vel_giro = parametro_msg;
          md.vmax = vel_giro;
          mi.vmax = vel_giro;
          if (md.estado == NOMINAL or md.estado == ACELERANDO)  //Solamente queremos que precalcule si se está moviendo en ese momento. Si no, simplemente se cambia la velocidad y cuando se vuelva a mover se aplicará
          {
            md.ini_marcha = true;  //Está hecho para que se cambie solo si está en movimiento y se desacelere o acelere.
          }
          if (mi.estado == NOMINAL or mi.estado == ACELERANDO)  //Solamente queremos que precalcule si se está moviendo en ese momento. Si no, simplemente se cambia la velocidad y cuando se vuelva a mover se aplicará
          {
            mi.ini_marcha = true;  //Está hecho para que se cambie solo si está en movimiento y se desacelere o acelere.
          }
        }
        break;

      case MOD_VEL:
        if (len_msg > 1)  //Por seguridad no vaya a ser que no se haya enviado nada
        {
          if (parametro_msg < VEL_MIN) parametro_msg = VEL_MIN;  //Por seguridad
          if (parametro_msg > VEL_MAX) parametro_msg = VEL_MAX;
          if (parametro_msg2 < VEL_MIN) parametro_msg2 = VEL_MIN;  //Por seguridad
          if (parametro_msg2 > VEL_MAX) parametro_msg2 = VEL_MAX;

          mi.vmax = parametro_msg;
          md.vmax = parametro_msg2;
          if (separador == -1) md.vmax = mi.vmax;

          if (md.estado == NOMINAL or md.estado == ACELERANDO)  //Solamente queremos que precalcule si se está moviendo en ese momento. Si no, simplemente se cambia la velocidad y cuando se vuelva a mover se aplicará
          {
            md.ini_marcha = true;  //Está hecho para que se cambie solo si está en movimiento y se desacelere o acelere.
          }
          if (mi.estado == NOMINAL or mi.estado == ACELERANDO)  //Solamente queremos que precalcule si se está moviendo en ese momento. Si no, simplemente se cambia la velocidad y cuando se vuelva a mover se aplicará
          {
            mi.ini_marcha = true;  //Está hecho para que se cambie solo si está en movimiento y se desacelere o acelere.
          }
        }
        break;

      case CM_IZQ:
        md.vmax = vel_adel;
        mi.vmax = vel_adel;
        if (len_msg > 1)  //Por seguridad no vaya a ser que no se haya enviado nada
        {
          recorrido_cm_izq = parametro_msg;
          if (mi.estado == INMOVIL and !shut_down) {  //Solo si estamos inmoviles le hacemos caso
            mi.dire_actual = FORWARD;
            mi.ini_marcha = true;
            mi.mov_continuo = false;
            mi.estado = ACELERANDO;  //Suponemos que empezamos parados y cambiamos a acelerar.
          }
        }
        break;

      case CM_DCH:
        md.vmax = vel_adel;
        mi.vmax = vel_adel;
        if (len_msg > 1)  //Por seguridad no vaya a ser que no se haya enviado nada
        {
          recorrido_cm_dch = parametro_msg;
          if (md.estado == INMOVIL and !shut_down) {  //Solo si estamos inmoviles le hacemos caso
            md.dire_actual = FORWARD;
            md.ini_marcha = true;
            md.mov_continuo = false;
            md.estado = ACELERANDO;  //Suponemos que empezamos parados y cambiamos a acelerar.
          }
        }
        break;

      case CM:
        md.vmax = vel_adel;
        mi.vmax = vel_adel;
        if (len_msg > 1)  //Por seguridad no vaya a ser que no se haya enviado nada
        {
          recorrido_cm_izq = parametro_msg;
          recorrido_cm_dch = recorrido_cm_izq;
          if (mi.estado == INMOVIL and md.estado == INMOVIL and !shut_down) {  //Solo si estamos inmoviles le hacemos caso
            mi.dire_actual = FORWARD;
            md.dire_actual = FORWARD;
            mi.ini_marcha = true;
            md.ini_marcha = true;
            mi.mov_continuo = false;
            md.mov_continuo = false;
            mi.estado = ACELERANDO;  //Suponemos que empezamos parados y cambiamos a acelerar.
            md.estado = ACELERANDO;  //Suponemos que empezamos parados y cambiamos a acelerar.
          }
        }
        break;

      case SHUT_DOWN:

        if (md.estado == INMOVIL and mi.estado == INMOVIL) {
          shut_down = !shut_down;
          digitalWrite(pin_shut_down, shut_down);
        }
        break;
    }
    nueva_pet_tipo = CUMPLIDA;  //No queremos entrar de nuevo en este if.
    msg = "";
  }




  if ((micros() - mi.antes) >= back2workIzq) {
    if (mi.estado != INMOVIL) {  //Con esta condición nos aseguramos que no entramos más en la función si nadie cambia la cond de INMÓVIL
      mi.estadoMotor(recorrido_cm_izq, mi.dire_actual);
    }
  }

  if ((micros() - md.antes) >= back2workDch) {
    if (md.estado != INMOVIL)
      md.estadoMotor(recorrido_cm_dch, md.dire_actual);
  }


  if (mi.recalculo) {  //Con esta condición nos aseguramos que no entramos más en la función si nadie cambia la cond de INMÓVIL
    antesCalRampIz = micros();
    back2workIzq = mi.rampGenerator();
    antesCalRampIz = micros() - antesCalRampIz;


    /* if(back2workIzq != ToffNominal){
        res = 50600.0/back2workIzq;
        Serial.print(res);
        Serial.print(",");
      }*/

    /*Serial.print("EM: ");
      Serial.println(mi.estado);*/
  }




  if (md.recalculo) {
    antesCalRampDch = micros();
    back2workDch = md.rampGenerator();

    antesCalRampDch = micros() - antesCalRampDch;

    /* if (back2workDch != ToffNominal) {
      res2 = 50600.0 / (back2workDch+ToffNominal);
      Serial.print(res2);
      Serial.println();
    }*/
    /*Serial.print("Dch: ");
      Serial.println(antesCalRampDch);
      Serial.print(" b2w: ");
      Serial.print(back2workDch);*/
    /*Serial.print(" EM: ");
    Serial.println(md.estado);*/
  }
  enviarEstado();
}

/*** Eventos de comunicación o HANDLER ****/

// Se ejecuta cada vez que el máster nos envía datos. Cuanto menos carga le metamos mejor
void Cuentame(int num_bytes) {
  byte identifica_pet;
  while (Wire.available() > 0) {
    msg = msg + (char)Wire.read();
  }
  identifica_pet = index_libreria(msg.charAt(0));  //En que posición está la Orden recibida

  switch (identifica_pet) {  //Tomamos el caracter identificador, lo buscamos en Libreria y devuelve el índice
    case PARAR:
      nueva_pet_tipo = ORDEN;
      pet = PARAR;
      break;

    case PARAR_DE_INMEDIATO:
      nueva_pet_tipo = ORDEN;
      pet = PARAR_DE_INMEDIATO;
      break;

    case ADELANTE:
      pet = ADELANTE;
      nueva_pet_tipo = ORDEN;
      break;

    case GIR_DCH:
      pet = GIR_DCH;
      nueva_pet_tipo = ORDEN;
      break;

    case GIR_IZQ:
      pet = GIR_IZQ;
      nueva_pet_tipo = ORDEN;
      break;

    case ATRAS:
      pet = ATRAS;
      nueva_pet_tipo = ORDEN;
      break;

    case MOD_VEL_IZQ:
      pet = MOD_VEL_IZQ;
      nueva_pet_tipo = ORDEN;
      break;

    case MOD_VEL_DCH:
      pet = MOD_VEL_DCH;
      nueva_pet_tipo = ORDEN;
      break;

    case MOD_VEL_GIRO:
      pet = MOD_VEL_GIRO;
      nueva_pet_tipo = ORDEN;
      break;

    case MOD_VEL:
      pet = MOD_VEL;
      nueva_pet_tipo = ORDEN;
      break;

    case MOD_GRAD:  //TODO: Esto no está claro que nos sirva de algo
      pet = MOD_GRAD;
      nueva_pet_tipo = ORDEN;
      break;

    case CM_IZQ:
      pet = CM_IZQ;
      nueva_pet_tipo = ORDEN;
      break;

    case CM_DCH:
      pet = CM_DCH;
      nueva_pet_tipo = ORDEN;
      break;

    case CM:
      pet = CM;
      nueva_pet_tipo = ORDEN;
      break;

    case STEPS_IZQ:
      itoa(mi.steps, prox_msg, 10);
      pet = STEPS_IZQ;
      nueva_pet_tipo = INFORMACION;
      break;

    case STEPS_DCH:
      itoa(md.steps, prox_msg, 10);  //  --> Pasamos de int a array de char
      pet = STEPS_DCH;
      nueva_pet_tipo = INFORMACION;
      break;

    case STEPS:
      itoa(md.steps, prox_msg, 10);
      //itoa(mi.steps, (prox_msg+2), 10);
      pet = STEPS;
      nueva_pet_tipo = INFORMACION;
      break;

    case SHUT_DOWN:

      pet = SHUT_DOWN;
      nueva_pet_tipo = ORDEN;
      break;
  }
}

/*Siempre que haya un cambio de estado se pregunta si se ha cambiado a alguno interesante que queramos enviar*/
void enviarEstado() {

  if (estadoAnterior != md.estado) {
    if (md.estado == INMOVIL || md.estado == PARANDO_EMER) pedirPermiso = true;
    if (md.estado == PARANDO) {
      if (md.dire_actual == mi.dire_actual) pedirPermiso = true;  //Esto es para que no salten las luces de parada si se estaba girando
    }
    if (md.estado == NOMINAL) {
      if (md.dire_actual == mi.dire_actual) pedirPermiso = true;  //Esto es para que no salten las luces de parada si se estaba girando
    }
    estadoAnterior = md.estado;
  }

  if (pedirPermiso != permisoActivo) {
    permisoActivo = pedirPermiso;
    if (pedirPermiso) {
      itoa(md.estado, prox_msg, 10);
      itoa(md.vmax, &prox_msg[1], 10);
    }
    digitalWrite(pinPedirPermiso, pedirPermiso);
  }
}
// Se ejecuta cada vez que el máster nos pida datos. Enviamos el mensaje ya preparado

void TeCuento() {
  Wire.write(prox_msg);
  pedirPermiso = false;
  msg = ""; /*Esto es jodidamente importante porque en el void loop no se puede vaciar el msg y se quedá igual para siempre por la cara*/
}