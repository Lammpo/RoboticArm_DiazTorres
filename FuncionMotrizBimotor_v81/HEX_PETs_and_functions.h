/*Direcciones hexadecimales para el mando IRremote*/
#define BOTONderecha 0xF906FB04
#define BOTONizquierda 0xF807FB04
#define BOTONadelante 0xBF40FB04
#define BOTONstop 0xBE41FB04      //atras
#define BOTONparadaEmer 0xBB44FB04    // Intro
#define BOTONShutDown 0xF708FB04  //power
#define BOTONcm 0xD728FB04        //back
#define BOTONmod_vel 0xA45BFB04   //exit
#define BOTONtileBorrar 0x847BFB04 // Tile entre exit y back
#define BOTONseparador 0x6699FB04       //auto entre menu y smenu

#define BOTON_1 0xEE11FB04
#define BOTON_2 0xED12FB04
#define BOTON_3 0xEC13FB04
#define BOTON_4 0xEB14FB04
#define BOTON_5 0xEA15FB04
#define BOTON_6 0xE916FB04
#define BOTON_7 0xE817FB04
#define BOTON_8 0xE718FB04
#define BOTON_9 0xE619FB04
#define BOTON_0 0xEF10FB04
/*
#define BOTONpag_anterior 0x1EE1FB04
#define BOTONpag_siguiente 0x1FE0FB04
#define BOTONmas_vol 0xFD02FB04
#define BOTONmenos_vol 0xFC03FB04
#define BOTONmenu 0xBC43FB04
#define BOTONSmenu 0xC03FFB04
#define BOTONauto 0x6699FB04
#define BOTONtile 0x847BFB04
*/
#define MAX_CHARS_MSG 6
char msg_mando[MAX_CHARS_MSG] = { ' ', '\0', '\0', '\0', '\0', '\0' };

/*Librería para el índice de órdenes*/
const char Libreria[25] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'w', 'y', 'z' };  //id de Orden o petición de información.
byte index_libreria(char _msg) {
  for (byte i = 0; i < 25; i++) {
    if (_msg == Libreria[i])
      return i;
  }
  return 0;
}
/*Lista de controladores*/
const char Controlador[4] = { 'A', 'B', 'C', 'D' };

/*PARÁMETROS MÍNIMOS Y MÁXIMOS*/
#define VEL_MAX 60
#define GRAD_MIN 0
#define VEL_MIN 20
/*PARÁMETROS INICIALES*/
#define VEL_INI 30
#define GRAD_INI 45
#define VEL_GIRO 25

/*Peticiones y tipos de mensaje*/
enum peticiones {
  PARAR,
  PARAR_DE_INMEDIATO,

  ADELANTE,
  GIR_DCH,
  GIR_IZQ,
  ATRAS,
  MOD_VEL_IZQ,
  MOD_VEL_DCH,
  MOD_VEL_GIRO,
  MOD_VEL,
  MOD_GRAD,

  CM_IZQ,
  CM_DCH,
  CM,

  STEPS_IZQ,
  STEPS_DCH,
  STEPS,

  SHUT_DOWN
};


enum TiposPeticion {
  INFORMACION,
  ORDEN,
  CAMBIO_PARAMETRO,
  CUMPLIDA
};

enum estado_motor {
  ACELERANDO,
  NOMINAL,
  FRENANDO,
  PARANDO,
  PARANDO_EMER,
  INMOVIL
};
