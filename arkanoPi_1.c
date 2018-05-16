
#include "arkanoPi_1.h"
#include "fsm.h"


static /*volatile*/ tipo_juego juego;
//static int momento = 0;

int serpiente = 0;// esta variable sirve para bloquear el movimiento de la serpiente cuando estamosm mostrando el resultado por pantalla  y seguimos en WAIT_PUSH

//------------------------------------------------------
// FUNCIONES DE ACCION
//------------------------------------------------------
enum fsm_state {
	WAIT_START=0,
	WAIT_PUSH=1,
	WAIT_END=2,
};
int columna=0;
int debounceTime=DEBOUNCE_TIME;
static tmr_t* tmrt;
static tmr_t* tmrp;

int col[7] = {0,1,2,3,4,7,23};

volatile int flags = 0;

void delay_until (unsigned int next) {
	unsigned int now = millis();

	if (next > now) {
		delay (next - now);
	}
}
int CompruebaTeclaPulsada (fsm_t* this) {//este metodo devuelve un 1 si el flag que comprueba esta activado
	//asi la matriz de estados es capaz de saber los flags que han sido activados
	int result;

	piLock (FLAGS_KEY);
	result = (flags & FLAG_TECLA);
	piUnlock (FLAGS_KEY);

	return result;
}

int CompruebaTeclaPelota (fsm_t* this) {//este metodo devuelve un 1 si el flag que comprueba esta activado
	//asi la matriz de estados es capaz de saber los flags que han sido activados
	int result;

	piLock (FLAGS_KEY);
	result = (flags & FLAG_PELOTA);
	piUnlock (FLAGS_KEY);

	return result;
}

int CompruebaTeclaRaquetaIzquierda (fsm_t* this) {//este metodo devuelve un 1 si el flag que comprueba esta activado
	//asi la matriz de estados es capaz de saber los flags que han sido activados
	int result;

	piLock (FLAGS_KEY);
	result = (flags & FLAG_RAQUETA_IZQUIERDA);
	piUnlock (FLAGS_KEY);

	return result;
}

int CompruebaTeclaRaquetaDerecha (fsm_t* this) {//este metodo devuelve un 1 si el flag que comprueba esta activado
	//asi la matriz de estados es capaz de saber los flags que han sido activados
	int result;

	piLock (FLAGS_KEY);
	result = (flags & FLAG_RAQUETA_DERECHA);
	piUnlock (FLAGS_KEY);

	return result;
}
int CompruebaFinalJuego (fsm_t* this) {//este metodo devuelve un 1 si el flag que comprueba esta activado
	//asi la matriz de estados es capaz de saber los flags que han sido activados
	int result;

	piLock (FLAGS_KEY);
	result = (flags & FLAG_FINAL_JUEGO);
	piUnlock (FLAGS_KEY);

	return result;
}


/* void InicializaJuego (fsm_t *fsm): funcion encargada de llevar a cabo
 la oportuna inicializaciÛn de toda variable o estructura de datos
 que resulte necesaria para el desarrollo del juego.*/
void InicializaJuego (fsm_t *fsm) {
	piLock (FLAGS_KEY);
	flags &= ~FLAG_TECLA;
	flags &= ~FLAG_RAQUETA_IZQUIERDA;
	flags &= ~FLAG_RAQUETA_IZQUIERDA;
	piUnlock (FLAGS_KEY);
	piLock (STD_IO_BUFFER_KEY);
	ActualizaPantallaSnake(&juego.snake);//actualizamos todas las variables del snake
	PintaPantallaPorTerminalSnake(&juego.snake.pantalla);//pintamos por pantalla para que deje de salir el mensaje inicial
	piUnlock (STD_IO_BUFFER_KEY);



}

/* void MueveRaquetaIzquierda (fsm_t *fsm): funcion encargada de ejecutar
 el movimiento antihorario de la serpiente*/
void MueveRaquetaIzquierda (fsm_t *fsm) {
	piLock (FLAGS_KEY);
	flags &= ~FLAG_RAQUETA_IZQUIERDA;
	flags &= ~FLAG_TECLA;
	piUnlock (FLAGS_KEY);
	if (juego.snake.serpiente.v == 1){//cambiamos la direciÛn de la serpiente
		juego.snake.serpiente.v = 4;
	}else{
		juego.snake.serpiente.v--;
	}


}

/* void MueveRaquetaDerecha (fsm_t *fsm): funcion encargada de ejecutar
 el movimiento horario de la serpiente*/
void MueveRaquetaDerecha (fsm_t *fsm) {
	piLock (FLAGS_KEY);
	flags &= ~FLAG_RAQUETA_DERECHA;
	flags &= ~FLAG_TECLA;
	piUnlock (FLAGS_KEY);
	if (juego.snake.serpiente.v == 4){//cambiamos la direciÛn de la serpiente
		juego.snake.serpiente.v = 1;
	}else{
		juego.snake.serpiente.v++;
	}
}



/* void MovimientoPelota (fsm_t *fsm): funciÛn encargada de actualizar la
 posiciÛn de la serpiente conforme a la direccion marcada.
 Para ello deber· mover correctamente la cabeza y la cola
 detectadno si se choca contra los lites del mapa o contra si misma
 o si por el contrario se come una manzana.*/
void MovimientoPelota (fsm_t *fsm) {
	piLock (FLAGS_KEY);
	flags &= ~FLAG_PELOTA;
	flags &= ~FLAG_TECLA;
	flags &= ~FLAG_RAQUETA_DERECHA;
	flags &= ~FLAG_RAQUETA_IZQUIERDA;
	piUnlock (FLAGS_KEY);

	if (serpiente == 1){//si se esta escribiendo el mensaje de final de partida no se hace nada
		return;
	}

	//ponemos la v en la posicion 0 que marca la posicion nueva de la cabeza respecto a la anterior
	juego.snake.serpiente.pos[0] = juego.snake.serpiente.v;
	//movemos la matriz de posiciones ya que el gusano se mueve de la misma forma
	int i = 0;
	for(i=juego.snake.serpiente.lon;i>0;i--) {
		juego.snake.serpiente.pos[i] = juego.snake.serpiente.pos[i-1];
	}
	//cambiamos la posicion de la cabeza
	switch(juego.snake.serpiente.v){
	case(1):
		juego.snake.serpiente.xi--;
		break;
	case(2):
		juego.snake.serpiente.yi--;
		break;
	case(3):
		juego.snake.serpiente.xi++;
		break;
	case(4):
		juego.snake.serpiente.yi++;
		break;
	}
	int aumento = 0;
	//si la nueva posicion esta fuera del mapa derrota
	if(juego.snake.serpiente.xi > 9 || juego.snake.serpiente.xi < 0){
		Perdiste();
	}else if(juego.snake.serpiente.yi > 6 || juego.snake.serpiente.yi < 0){
		Perdiste();
	//si la nueva posicion tiene un 1
	}else if(juego.snake.pantalla.matriz[juego.snake.serpiente.xi][juego.snake.serpiente.yi]==1 ){
		//puede ser manzana
		if (juego.snake.serpiente.xi == juego.snake.manzana.x && juego.snake.serpiente.yi == juego.snake.manzana.y){
			//comemos
			juego.snake.manzana.n++;
			//si hemos comido un multiplo de 3 nos aumentamos
			if (juego.snake.manzana.n%3 == 0){
				aumento = 1;
				juego.snake.serpiente.lon++;
			}
			//movemos la manzana. No puede haber una manzana sobre la serpiente
			juego.snake.manzana.x = juego.snake.manzana.x + 3;
			if (juego.snake.manzana.x >= 7){
				juego.snake.manzana.x = juego.snake.manzana.x % 7;
				juego.snake.manzana.y ++;
			}
			if(juego.snake.manzana.y > 9){
				juego.snake.manzana.y = 0;
			}
			//nos aseguramos de que la manzana no se pone encima de la serpiente
			while (juego.snake.pantalla.matriz[juego.snake.manzana.x][juego.snake.manzana.y]==1){
				juego.snake.manzana.x++;
				if (juego.snake.manzana.x >= 7){
					juego.snake.manzana.x = juego.snake.manzana.x % 7;
					juego.snake.manzana.y ++;
				}
				if(juego.snake.manzana.y > 9){
					juego.snake.manzana.y = 0;
				}
			}
			//juego.snake.pantalla.matriz[juego.snake.manzana.x][juego.snake.manzana.y] = 1;
			//pintamos la nueva cabeza
			juego.snake.serpiente.cola.matriz[juego.snake.serpiente.xi][juego.snake.serpiente.yi] = 1;
			//pintamos la cola
			if (aumento == 0){
				juego.snake.serpiente.cola.matriz[juego.snake.serpiente.xf][juego.snake.serpiente.yf] = 0;
				switch (juego.snake.serpiente.pos[juego.snake.serpiente.lon]){
				case(1):
						juego.snake.serpiente.xf--;
						break;
					case(2):
						juego.snake.serpiente.yf--;
						break;
					case(3):
						juego.snake.serpiente.xf++;
						break;
					case(4):
						juego.snake.serpiente.yf++;
					break;
				}
			}
		//es la serpiente
		}else{
			Perdiste();
		}
	//mapa libre
	}else{
		//pintamos la cabeza
		juego.snake.serpiente.cola.matriz[juego.snake.serpiente.xi][juego.snake.serpiente.yi] = 1;
		//pintamos la cola
		juego.snake.serpiente.cola.matriz[juego.snake.serpiente.xf][juego.snake.serpiente.yf] = 0;
		switch (juego.snake.serpiente.pos[juego.snake.serpiente.lon]){
			case(1):
				juego.snake.serpiente.xf--;
				break;
			case(2):
				juego.snake.serpiente.yf--;
				break;
			case(3):
				juego.snake.serpiente.xf++;
				break;
			case(4):
				juego.snake.serpiente.yf++;
			break;
		}
	}
	ActualizaPantallaSnake(&juego.snake);//actualizamos pantalla para que se pinte correctamente
	piLock (STD_IO_BUFFER_KEY);
	PintaPantallaPorTerminalSnake(&juego.snake.pantalla);
	piUnlock (STD_IO_BUFFER_KEY);
}


/* void perdiste (void): Metodo que se encarga de gestionar
 * todos los procesos necesarios despues de la derrota*/
void Perdiste(void){
	serpiente = 1;//ponemos esto a 1 para poder imprimir el mensaje de derrota
	SnakeDerrota(&juego.snake);//metodo de la clase snake que realiza todo lo necesario cuando se pierde la partida
	sleep(2);//tiempo que esta el mensaje en pantala
	piLock (FLAGS_KEY);
	flags |= FLAG_FINAL_JUEGO;//ativamos para pasar a WAIT_END
	piUnlock (FLAGS_KEY);
}

/* void FinalJuego (fsm_t *fsm): funci√≥n encargada de mostrar en la ventana de
* terminal los mensajes necesarios para informar acerca del resultado del juego.*/
void FinalJuego (fsm_t *fsm) {
	piLock (FLAGS_KEY);
	flags &= ~FLAG_FINAL_JUEGO;
	flags &= ~FLAG_TECLA;
	piUnlock (FLAGS_KEY);
	piLock (STD_IO_BUFFER_KEY);
	printf ("PERDISTE %d MANZANAS COMIDAS", juego.snake.manzana.n);
	piUnlock (STD_IO_BUFFER_KEY);
}

/*void ReseteaJuego (void): funci√≥n encargada de llevar a cabo la
* reinicializaci√≥n de cuantas variables o estructuras resulten
* necesarias para dar comienzo a una nueva partida.*/
void ReseteaJuego (fsm_t *fsm) {
	piLock (FLAGS_KEY);
	flags &= ~FLAG_FINAL_JUEGO;
	flags &= ~FLAG_TECLA;
	piUnlock (FLAGS_KEY);
	serpiente = 0;
	InicializaSnake(&juego.snake);//metodo que pinta el mensaje por pantalla e inicializa el juego correctamente
	piLock (STD_IO_BUFFER_KEY);
	PintaPantallaPorTerminalSnake(&juego.snake.pantalla);//pintamos mensaje inicial
	piUnlock (STD_IO_BUFFER_KEY);

}

//------------------------------------------------------
// FUNCIONES DE INICIALIZACION
//------------------------------------------------------

/* int systemSetup (void): procedimiento de configuracion del sistema.
* configura el uso de posibles librerias ,
* configurar las interrupciones externas asociadas a los pines GPIO,
* configurar las interrupciones peri√≥dicas y sus correspondientes temporizadores,
* crear, si fuese necesario, los threads adicionales que pueda requerir el sistema*/
int systemSetup (void) {

	wiringPiSetupGpio();
	piLock (FLAGS_KEY);
	flags = 0;
	piUnlock (FLAGS_KEY);
	tmrt = tmr_new (refrescar);//para pintar la matriz de leds
	tmrp = tmr_new (flagPelota);//para mover la pelota
	tmr_startms (tmrt, 1);
	tmr_startms (tmrp, 1000);
	//filas
	pinMode(0,OUTPUT);
	pinMode(1,OUTPUT);
	pinMode(2,OUTPUT);
	pinMode(3,OUTPUT);
	pinMode(4,OUTPUT);
	pinMode(7,OUTPUT);
	pinMode(23,OUTPUT);
	//columnas
	pinMode(22,OUTPUT);
	pinMode(18,OUTPUT);
	pinMode(17,OUTPUT);
	pinMode(14,OUTPUT);
	//botones
	pinMode (GPIO_BUTTONDER, INPUT);
	pinMode (GPIO_BUTTONIZQ, INPUT);
	wiringPiISR (GPIO_BUTTONDER, INT_EDGE_FALLING, boton_izquierda);
	wiringPiISR (GPIO_BUTTONIZQ, INT_EDGE_FALLING, boton_derecha);

	return 1;

}
/*static void flagPelota (union sigval value)
 * aqui se activa el flag del snake cada segundo
 */
static void flagPelota (union sigval value){
	piLock (FLAGS_KEY);
	flags |=FLAG_PELOTA;//flag que activa el movimiento pelota
	piUnlock (FLAGS_KEY);
	tmr_startms (tmrp, 1000);
}

void boton_izquierda(void){
	if (millis () < debounceTime) {
	debounceTime = millis () + DEBOUNCE_TIME ;
	return;
	}
	piLock (FLAGS_KEY);
	flags |=FLAG_RAQUETA_IZQUIERDA;
	flags |= FLAG_TECLA;
	piUnlock (FLAGS_KEY);

	while (digitalRead (GPIO_BUTTONIZQ) == HIGH) {
	delay (1) ;
	}
	debounceTime = millis () + DEBOUNCE_TIME ;

}
void boton_derecha(void){
	if (millis () < debounceTime) {
	debounceTime = millis () + DEBOUNCE_TIME ;
	return;
	}
	piLock (FLAGS_KEY);
	flags |=FLAG_RAQUETA_DERECHA;
	flags |= FLAG_TECLA;
	piUnlock (FLAGS_KEY);
	while (digitalRead (GPIO_BUTTONDER) == HIGH) {
	delay (1) ;
	}
	debounceTime = millis () + DEBOUNCE_TIME ;

}


static void  refrescar(union sigval value){


	int i;
	int c= 9 - (columna%10);

	for(i=0;i<7;i++){
		if(juego.snake.pantalla.matriz[c][i] == 1){
			digitalWrite(col[i], LOW);
		}else{
			digitalWrite(col[i], HIGH);
		}
	}
	//col
	digitalWrite(14,c & 0x01);
	digitalWrite(17,c & 0x02);
	digitalWrite(18,c & 0x04);
	digitalWrite(22,c & 0x08);
	tmr_startms (tmrt, 1);
	columna++;
}

int main () {
	unsigned int next;

	fsm_trans_t tabla_juego[] = {
			{ WAIT_START,   CompruebaTeclaPulsada,  WAIT_PUSH, InicializaJuego},
			{ WAIT_PUSH,   CompruebaTeclaRaquetaIzquierda,  WAIT_PUSH, MueveRaquetaIzquierda},
			{ WAIT_PUSH,   CompruebaTeclaPelota,  WAIT_PUSH, MovimientoPelota},
			{ WAIT_PUSH,   CompruebaTeclaRaquetaDerecha,  WAIT_PUSH, MueveRaquetaDerecha },
			{ WAIT_PUSH,   CompruebaFinalJuego,  WAIT_END, FinalJuego},
			{ WAIT_END,   CompruebaTeclaPulsada,  WAIT_START, ReseteaJuego},
			{ -1, NULL, -1, NULL },
	};


	fsm_t* juego_fsm = fsm_new (WAIT_START, tabla_juego, NULL);
	// Configuracion e inicializacion del sistema

	systemSetup();
	ReseteaJuego(juego_fsm);
	/*fsm_setup (juego_fsm);*/

	next = millis();
	while (1) {
		fsm_fire (juego_fsm);
		next += CLK_MS;
		delay_until (next);
	}

	fsm_destroy (juego_fsm);


}
