/*
 * snake_1.h
 *
 *  Created on: 9/4/2017
 *      Author: Carlos
 */

#ifndef SNAKE_1_H_
#define SNAKE_1_H_

#include <stdio.h>

#define MATRIZ_ANCHO 	10
#define MATRIZ_ALTO 	7
#define LONGITUD_BASE 	7
#define LONGITUD_MAX    40



typedef struct {//pantalla global
	int matriz[MATRIZ_ANCHO][MATRIZ_ALTO];
} tipo_pantalla;

typedef struct {
	//longitud de la serpiente
	int lon;
	//CABEZA
	int yi;
	int xi;
	//direccion de la cabeza
	int v;
	//CULO
	int yf;
	int xf;
	//representa la forma de la serpiente
	tipo_pantalla cola;
	//historail de movimientos
	int pos[LONGITUD_MAX] ;
} tipo_serpiente;

typedef struct {//caracteristicas de la manzana
	int x;
	int y;
	//numero de manzanas comidas
	int n;
} tipo_manzana;

typedef struct {
  tipo_serpiente serpiente;
  tipo_pantalla pantalla;
  tipo_manzana manzana;
} tipo_snake;

void ReseteaMatriz(tipo_pantalla *p_pantalla);
void ReseteaManzana(tipo_manzana *p_manzana);
void ReseteaSerpiente(tipo_serpiente *p_serpiente);
void PintaPantallaPorTerminalSnake(tipo_pantalla *p_pantalla);
void PintaManzanaPorTerminal(tipo_manzana *p_manzana, tipo_pantalla *p_pantalla);
void PintaSerpiente(tipo_serpiente *p_serpiente, tipo_pantalla *p_pantalla);
void ActualizaPantallaSnake(tipo_snake* p_snake);
void PintaMensajeInicialPantalla (tipo_pantalla *p_pantalla, tipo_pantalla *p_pantalla_inicial) ;
void SnakeDerrota(tipo_snake *p_snake);
void InicializaSnake(tipo_snake *p_snake);

#endif /* SNAKE_1_H_ */
