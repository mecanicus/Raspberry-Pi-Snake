/*
 * snake_1.c
 *
 *  Created on: 9/4/2017
 *      Author: Carlos
 */
#include "snake_1.h"

int snake_basico[MATRIZ_ANCHO][MATRIZ_ALTO] = {
		//matriz que representa la posicion inicial de la serpiente
		{0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0},
		{0,0,0,0,1,0,0},
		{0,0,0,0,1,0,0},
		{0,0,0,0,1,0,0},
		{0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0},
};

void ReseteaMatriz(tipo_pantalla *p_pantalla) {
	//pone la matriz principal a todo 0 para poder pintar en ella
	int i, j = 0;

	for(i=0;i<MATRIZ_ANCHO;i++) {
		for(j=0;j<MATRIZ_ALTO;j++) {
			p_pantalla->matriz[i][j] = 0;
		}
	}
}

void ReseteaManzana(tipo_manzana *p_manzana) {
	//ponemos la manzana incialmente en esa posicion
	p_manzana->x = MATRIZ_ANCHO/4 - 1;
	p_manzana->y = MATRIZ_ALTO/4;
	p_manzana->n = 0;
}

void ReseteaSerpiente(tipo_serpiente *p_serpiente) {
	// serpiente en poscion definida en el array de arriba y con direccion hacia la izquierda
	p_serpiente->xi = 4;
	p_serpiente->yi = 4;
	p_serpiente->xf = 6;
	p_serpiente->yf = 4;
	p_serpiente->lon = 3;
	p_serpiente->v = 1;
	//pintamos la forma del snake en la matriz qeu lo representa
	int i, j = 0;
		for(i=0;i<MATRIZ_ANCHO;i++) {
			for(j=0;j<MATRIZ_ALTO;j++) {
				p_serpiente->cola.matriz[i][j] = snake_basico[i][j];
			}
		}
	i = 3;
	for(i=3;i<LONGITUD_MAX;i++) {
		p_serpiente->pos[1]=0;
	}
	//historial del movimientos todo a la derecha
	p_serpiente->pos[0] = 1;
	p_serpiente->pos[1] = 1;
	p_serpiente->pos[2] = 1;
}

void PintaPantallaPorTerminalSnake(tipo_pantalla *p_pantalla) {
	//para pintar el snake en el terminal
	int i, j=0;
		puts("[PANTALLA]\n");
		for(i=0;i<MATRIZ_ALTO;i++) {
					for(j=0;j<MATRIZ_ANCHO;j++) {
						printf("%d",p_pantalla->matriz[j][i]);
					}
					printf("\n");

		}
	}

void PintaManzanaPorTerminal(tipo_manzana *p_manzana, tipo_pantalla *p_pantalla){
	//pinta un 1 donde esta la manzana
	if( (p_manzana->x >= 0) && (p_manzana->x < MATRIZ_ANCHO) ) {
			if( (p_manzana->y >= 0) && (p_manzana->y < MATRIZ_ALTO) ) {
				p_pantalla->matriz[p_manzana->x][p_manzana->y] = 1;
			}
			else {
				printf("\n\nPROBLEMAS!!!! posicion y=%d de la pelota INVALIDA!!!\n\n", p_manzana->y);
				fflush(stdout);
			}
		}
		else {
			printf("\n\nPROBLEMAS!!!! posicion x=%d de la pelota INVALIDA!!!\n\n", p_manzana->x);
			fflush(stdout);
		}
}

//IMPORTANTE LLAMAR ANTES QUE A MANZANA
void PintaSerpiente(tipo_serpiente *p_serpiente, tipo_pantalla *p_pantalla) {
	//pone toda la pantalla princiapal a la pantalla de la serpiente
	int i, j = 0;
	for(i=0;i<MATRIZ_ANCHO;i++) {
		for(j=0;j<MATRIZ_ALTO;j++) {
			p_pantalla->matriz[i][j] = p_serpiente->cola.matriz[i][j];
		}
	}

}

void ActualizaPantallaSnake(tipo_snake* p_snake) {
    // Borro toda la pantalla
	ReseteaMatriz((tipo_pantalla*)(&(p_snake->pantalla)));

	//pintamos primeor la serpiente y luego la pantalla
	PintaSerpiente(&(p_snake->serpiente), &(p_snake->pantalla));
	PintaManzanaPorTerminal(&(p_snake->manzana), &(p_snake->pantalla));

}

void PintaMensajeInicialPantalla (tipo_pantalla *p_pantalla, tipo_pantalla *p_pantalla_inicial) {
	//pintamos el mensaje inicial
	int i,j = 0;
	for(i=0;i<MATRIZ_ANCHO;i++) {
			for(j=0;j<MATRIZ_ALTO;j++) {
				p_pantalla->matriz[i][j] = p_pantalla_inicial->matriz[i][j];
			}
	    }
}

void SnakeDerrota(tipo_snake *p_snake){
	//pintamos por pantalla un mensaje de derrota
	static tipo_pantalla mensaje_inicial = { {
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{1,1,0,0,0,0,0},
			{1,1,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{1,1,0,0,0,0,0},
			{1,1,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
	} };

	ReseteaMatriz((tipo_pantalla*)(&(p_snake->pantalla)));
	PintaMensajeInicialPantalla(&p_snake->pantalla, &mensaje_inicial);
}

void InicializaSnake(tipo_snake *p_snake) {
	//pintamos el mensaje inicial pòr la pantalla
	//y reseteamos todos los demas objetos de la estructura de datos
	static tipo_pantalla mensaje_inicial = { {
			{0,0,0,0,0,0,0},
			{0,0,0,1,0,0,0},
			{1,1,0,1,1,0,0},
			{1,1,0,0,1,1,0},
			{0,0,0,0,0,1,1},
			{0,0,0,0,0,1,1},
			{1,1,0,0,1,1,0},
			{1,1,0,1,1,0,0},
			{0,0,0,1,0,0,0},
			{0,0,0,0,0,0,0},
	} };
	ReseteaMatriz((tipo_pantalla*)(&(p_snake->pantalla)));

	ReseteaManzana((tipo_manzana*)(&(p_snake->manzana)));
	ReseteaSerpiente((tipo_serpiente*)(&(p_snake->serpiente)));

	PintaMensajeInicialPantalla(&p_snake->pantalla, &mensaje_inicial);

}


