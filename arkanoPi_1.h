#ifndef _ARKANOPI_H_
#define _ARKANOPI_H_

#include <time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <wiringPi.h>
//#include "arkanoPiLib.h"
#include "snake_1.h"

#include "kbhit.h" // para poder detectar teclas pulsadas sin bloqueo y leer las teclas pulsadas

#include "fsm.h" // para poder crear y ejecutar la máquina de estados
#include "tmr.h"
#define CLK_MS 10// PERIODO DE ACTUALIZACION DE LA MAQUINA ESTADOS

// FLAGS DEL SISTEMA
#define FLAG_TECLA					0x01
#define FLAG_PELOTA					0x02
#define FLAG_RAQUETA_IZQUIERDA 		0x04
#define FLAG_RAQUETA_DERECHA 		0x08
#define FLAG_FINAL_JUEGO 			0x10
#define GPIO_BUTTONIZQ 19
#define GPIO_BUTTONDER 16
#define DEBOUNCE_TIME 122

#define	FLAGS_KEY	1
#define	STD_IO_BUFFER_KEY	2


/*typedef enum {
	WAIT_START,
	WAIT_PUSH,
	WAIT_END} tipo_estados_juego;
*/
typedef struct {
	//tipo_arkanoPi arkanoPi;
	/*tipo_estados_juego estado;*/
	tipo_snake snake;
	char teclaPulsada;
} tipo_juego;

//------------------------------------------------------
// FUNCIONES DE ACCION
//------------------------------------------------------

void InicializaJuego (fsm_t *fsm);
void MueveRaquetaIzquierda (fsm_t *fsm);
void MueveRaquetaDerecha (fsm_t *fsm);
void MovimientoPelota (fsm_t *fsm);
void FinalJuego (fsm_t *fsm);
void ReseteaJuego (fsm_t *fsm);
void boton_derecha (void);
void boton_izquierda(void);
void Perdiste(void);
static void refrescar(union sigval value);
static void flagPelota (union sigval value);
//------------------------------------------------------
// FUNCIONES DE INICIALIZACION
//------------------------------------------------------
int systemSetup (void);
PI_THREAD (thread_explora_teclado);
#endif /* ARKANOPI_H_ */
