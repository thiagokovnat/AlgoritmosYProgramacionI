/*PRECONDICIONES: EL USUARIO DEBERA INGRESAR UN MOVIMIENTO A LA VEZ
 *POSCONDICIONES: DETERMINARA EL RESULTADO DEL JUEGO BASADO EN LOS MOVIMIENTOS DEL USUARIO
 */

#include <stdio.h>
#include <stdbool.h>
#include "copa.h"
#include <stdlib.h>
#include <time.h>

//CONSTANTES
#define ARRIBA 'w'
#define ABAJO 's'
#define IZQUIERDA 'a'
#define DERECHA 'd'

#define GANADOR 1
#define PERDEDOR -1
#define CONTINUA_JUEGO 0

#define IMPEDIMENTA 'I'
#define RIDDIKULUS 'R'
/*
 * pos: Imprimira por pantalla los codigos basicos del laberinto y la vida actual del jugador
 * pre: -
 */
void imprimir_instrucciones(jugador_t jugador){
	printf("AYUDAS:\n");
	printf(" (R) ---> Riddikulus, contraresta al BOGGART\n (P) ---> Pociones, suman 15 de vida \n (I) ---> Impedimenta, contraresta al ESCREGUTO\n");
	printf("\nOBSTACULOS:\n");
	printf(" (B) ---> Boggart, reduce 15 de vida salvo que se tenga RIDDIKULUS\n (A) ---> Acromantula, reduce 10 de vida\n (E) ---> Escreguto, reduce 20 de vida salvo que se tenga IMPEDIMENTA\n\n\n");
	printf("Jugador: J. Vida actual: %i\n\n",jugador.vida);
	printf("Hechizos dominados:\n");
	for (int i = 0; i < jugador.tope_ayudas; ++i){
		if(jugador.ayudas[i].codigo == IMPEDIMENTA)
			printf("Impedimenta\n");
		else if(jugador.ayudas[i].codigo == RIDDIKULUS)
			printf("Riddikulus\n");
	}
	printf("\n\n");
}
/*
 * pos: Pedira al usuario que ingrese una tecla
 * pre: -
 */
void pedir_movimiento(char* tecla, juego_t juego){
  printf("Tu turno (Use la tecla 'a' para moverse hacia la izquierda, 'd' para moverse hacia la derecha, 'w' para moverse hacia adelante y 's' para moverse hacia atras): \n");
  scanf(" %c", tecla);
  while(!es_movimiento_valido(&juego,(*tecla))){
    printf("Movimiento invalido, ingrese nuevamente: ");
    scanf(" %c", tecla);
  }
}
/*
 * pos: Informara del resultado del juego
 * pre: -
 */
void imprimir_ganador(juego_t juego){
	
	if(estado_juego(juego) == GANADOR)
		system("figlet VICTORIA");
	else if(estado_juego(juego) == PERDEDOR)
		system("figlet DERROTA");
}

int main(){

	srand((unsigned)time(NULL));
	juego_t juego;
	char direccion;
	char laberinto[TAMANIO][TAMANIO];
	inicializar_laberinto(&juego);
	bool turno_inicial = true;

	while(estado_juego(juego) == CONTINUA_JUEGO){
		system("clear");
		if(turno_inicial){
			printf("\nBienvenido al Laberinto. Su objetivo es buscar la copa escondida dentro del mismo.\n\n");
			printf("Cuidado! El laberinto esconde muchos peligros. Evitalos o perderas vida!\nPensa bien cada movimiento, ya que cada uno te restara 3 puntos de vida. \n\n");
			turno_inicial = false;
		}
		actualizar_laberinto(juego,laberinto);
		imprimir_instrucciones(juego.jugador);
		mostrar_laberinto(laberinto);
		pedir_movimiento(&direccion, juego);
		mover_jugador(&juego.jugador, direccion);
		mover_rival(&juego);
		actualizar_juego(&juego);	
	}

	imprimir_ganador(juego);
	return 0;
}