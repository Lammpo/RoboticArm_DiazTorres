/*
INTERPRETE DE COMANDOS DE CODIGO G
- Desglosa instrucciones de texto en: (i) instrucciónes G o M y su tipo; (ii) parámetros (F, R, S, X, Y, Z, N, I); (iii) los comentarios se identifican con ";"
*/

#include <string.h>
#include <stdlib.h>

/*
Recibe una instrucción como una cadena de caracteres terminada en '\0'.
Determina el tipo de comoando que es, los parámetros y ejecuta la función correspondiente.
*/
void execute_cmd(char *cmd) {
  unsigned int n;
  char instruction[10];
  double a, b, c, d, e;  //Coordenadas angulares - Inicializar a coordenadas actuales
  double x, y, z;        //Coordenadas lineales (requieren cinemática inversa)
  double f;              //Feed rate (velocidad)

  char param[10] = "";


  // Leemos el valor de los parámetros de la instrucción
  while (*cmd) {
    if (*cmd == ';') {
      // Si encontramos un comentario, directamente salimos del bucle
      break;
    } else if (*cmd != ' ') {
      // Si no encontramos un espacio, seguimos añadiendo caracteres a la variable 'param'
      strncat(param, cmd, 1);
    } else {
      // Establecemos el valor del parámetro correspondiente según el primer caracter
      //printf("%s\n", param);
      switch (param[0]) {
        case 'A':
          a = atof(param + 1);
          //printf("Coordenada A: %f\n", a);
          break;
        case 'B':
          b = atof(param + 1);
          //printf("Coordenada B: %f\n", b);
          break;
        case 'C':
          c = atof(param + 1);
          //printf("Coordenada C: %f\n", c);
          break;
        case 'D':
          d = atof(param + 1);
          //printf("Coordenada D: %f\n", d);
          break;
        case 'E':
          e = atof(param + 1);
          //printf("Coordenada E: %f\n", e);
          break;
        case 'F':
          f = atof(param + 1);
          //printf("Feed rate: %f\n", f);
          break;
        case 'X':
          x = atof(param + 1);
          //printf("Coordenada X: %f\n", x);
          break;
        case 'Y':
          y = atof(param + 1);
          //printf("Coordenada Y: %f\n", y);
          break;
        case 'Z':
          z = atof(param + 1);
          //printf("Coordenada Z: %f\n", z);
          break;
        case 'N':
          n = atoi(param + 1);  //Transformamos a entero el número quitando el caracter inicial
          //printf("Linea: %d\n", n);
          break;
        default:
          strcpy(instruction, param);
          //printf("Instruccion: %s\n", instruction);
      }
      param[0] = '\0';
    }

    cmd++;
  }
}