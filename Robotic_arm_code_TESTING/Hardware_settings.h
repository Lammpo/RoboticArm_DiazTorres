
/*** Aquí se listarán todos lo parámetros del hardware necesarios para:
  -La conversión entre steps y grados o mm
  -El pinOut de la placa controladora

***/

// REDUCCIONES Y MICROSTEPPING
// Habrá que sincronizar el microstepping con el hardware

//Base giratoria
#define REDUC_GIRO 3
#define MICROSTEPPING_GIRO 8

// Brazo1
#define reducBrazo1 50
#define MICROSTEPPING_BRAZO1 8


//PINOUT

// //Base giratoria
// #define dirPinstepperGiro 25
// #define stepPinstepperGiro 26
// #define enablePinstepperGiro 32


// // Brazo 1
// #define stepPinstepperBrazo 27
// #define dirPinstepperBrazo 14
// #define enablePinstepperBrazo 33

#define STEP_PIN_DRV1 5
#define STEP_PIN_DRV2 32
#define STEP_PIN_DRV3 26
#define STEP_PIN_DRV4 12

#define DIR_PIN_DRV1 17
#define DIR_PIN_DRV2 33
#define DIR_PIN_DRV3 27
#define DIR_PIN_DRV4 13

#define ENABLE_PIN_DRV1 16
#define ENABLE_PIN_DRV2 4
#define ENABLE_PIN_DRV3 25 
#define ENABLE_PIN_DRV4 14

// General parameters
#define MAX_SPEED 50 // Grados/s
#define STEPS_PER_REV 200
