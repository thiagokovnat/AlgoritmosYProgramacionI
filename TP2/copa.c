#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "laberinto.h"
#include "copa.h"

//CONSTANTES:
#define VIDA_POR_PASO -3
#define VIDA_MAXIMA 50
#define VIDA_A_MOSTRAR_COPA 15
#define SIN_VIDA 0

#define OBSTACULOS_INICIALES 3
#define ACROMANTULA 'A'
#define BOGGART 'B'
#define ESCREGUTO 'E'

#define DANIO_ACROMANTULA -10
#define DANIO_BOGGART -15
#define DANIO_ESCREGUTO -20

#define AYUDAS_INICIALES 6
#define IMPEDIMENTA 'I'
#define RIDDIKULUS 'R'
#define POCION 'P'
#define ESFINGE 'F'

#define VIDA_A_RECUPERAR_IMPEDIMENTA 0
#define VIDA_A_RECUPERAR_RIDDIKULUS 0
#define VIDA_A_RECUPERAR_POCION 15
#define VIDA_A_RECUPERAR_ESFINGE 0

#define COPA 'C'
#define JUGADOR 'J'
#define RIVAL 'G'

#define ABAJO 's'
#define ARRIBA 'w'
#define IZQUIERDA 'a'
#define DERECHA 'd'

#define GANA_JUGADOR 1
#define GANA_RIVAL -1
#define EN_CURSO 0
#define DISTANCIA_MANHATTAN 10

#define SIN_AYUDAS 0
#define SIN_PASOS 0
#define MAX_PASOS 4
#define BORDE_INFERIOR 0
#define BORDE_MAYOR 14

// pre: Dichas coordenadas estan dentro del rango TAMANIOxTAMANIO
// pos: Devuelve TRUE si las coordenadas recibidas son iguales
bool posiciones_iguales(coordenada_t coordenada1, coordenada_t coordenada2){

  return (coordenada1.fil == coordenada2.fil && coordenada1.col == coordenada2.col);
}

// pre: posicion esta dentro del rango [0, tope_obstaculos - 1]
//      tope_obstaculos esta dentro del rango [0, TOTAL_OBSTACULOS]
// pos: Eliminara un elemento dentro del vector obstaculos
void eliminar_obstaculo(obstaculo_t obstaculos[TOTAL_OBSTACULOS], int posicion, int* tope_obstaculos){

  for(int i = posicion; i < (*tope_obstaculos) - 1; i++)
     obstaculos[i] = obstaculos[i + 1];

  (*tope_obstaculos)--;
}

// pre: -
// pos: Modificara la vida del jugador
void modificar_vida(int* vida, int vida_a_modificar){

  if((*vida) + vida_a_modificar > VIDA_MAXIMA)
    (*vida) = VIDA_MAXIMA;

  else
    (*vida) += vida_a_modificar;
}

// pre: buscado debe ser un caracter correspondiente a un hechizo del juego
// pos: Devuelve TRUE si el jugador posee el hechizo buscado
bool tiene_hechizo(jugador_t jugador, char buscado){

  bool tiene_buscado = false;

  for (int i = 0; i < jugador.tope_ayudas; ++i){
    if(jugador.ayudas[i].codigo == buscado)
        tiene_buscado = true;
  }

  return tiene_buscado;
}

// pre: -
// pos: Actualizara todos los elementos correspondientes a los obstaculos, restara vida si cae en uno y no tiene el hechizo correspondiente
void actualizar_obstaculos(juego_t* juego){

  for (int i = 0; i < juego->tope_obstaculos; ++i){
    if(posiciones_iguales(juego->jugador.posicion, juego->obstaculos[i].posicion)){

      if(juego->obstaculos[i].codigo == ESCREGUTO && !tiene_hechizo(juego->jugador, IMPEDIMENTA))
        modificar_vida(&(juego->jugador.vida), juego->obstaculos[i].danio);

      else if(juego->obstaculos[i].codigo == BOGGART && !tiene_hechizo(juego->jugador, RIDDIKULUS))
        modificar_vida(&(juego->jugador.vida), juego->obstaculos[i].danio);

      else if (juego->obstaculos[i].codigo == ACROMANTULA)
        modificar_vida(&(juego->jugador.vida), juego->obstaculos[i].danio);

      eliminar_obstaculo(juego->obstaculos, i, &(juego->tope_obstaculos));
    }
  }
}

// pre: La posicion esta en el rango [0, tope_ayudas - 1]
//      tope_ayudas esta en el rango [0, TOTAL_AYUDAS]
// pos: Eliminara una ayuda del vector ayudas
void eliminar_ayuda(ayuda_t ayudas[TOTAL_AYUDAS], int posicion, int* tope_ayudas){

  for(int i = posicion; i < (*tope_ayudas) - 1; i++)
     ayudas[i] = ayudas[i + 1];

  (*tope_ayudas)--;
}

// pre: hechizo debe ser un hechizo correspondiente al juego
// pos: Agregara el hechizo al vector de ayudas del jugador
void aprender_hechizo(jugador_t* jugador, ayuda_t hechizo){

  jugador->ayudas[jugador->tope_ayudas] = hechizo;
  jugador->tope_ayudas++;
}

// pre: -
// pos: Actualizara las ayudas dentro del juego, sumara vida si el jugador cayo en una pocion, aprendera hechizos en caso de que haya caido en uno
void actualizar_ayudas(juego_t* juego){

  for(int i = 0; i < juego->tope_ayudas; i++){
    if(posiciones_iguales(juego->jugador.posicion, juego->ayudas[i].posicion)){

      if(juego->ayudas[i].codigo == POCION)
        modificar_vida(&(juego->jugador.vida), juego->ayudas[i].vida_a_recuperar);

      else
        aprender_hechizo(&(juego->jugador), juego->ayudas[i]);

      eliminar_ayuda(juego->ayudas, i, &(juego->tope_ayudas));
    }
  }
}

// pre: Dichas coordenadas estan dentro del rango TAMANIOxTAMANIO
// pos: Validara que la distancia entre la coordenada recibida y la de la copa sea la pedida
bool distancia_valida(coordenada_t coordenada, coordenada_t copa){

    int modulo_fila = abs(copa.fil - coordenada.fil);
    int modulo_col = abs(copa.col - coordenada.col);

    return ((modulo_col + modulo_fila) > DISTANCIA_MANHATTAN);
}

// pre: La fila y la columna son enteros dentro del rango [0,TAMANIO]
// pos: Validara que la posicion este vacia
bool coordenada_valida(char laberinto_aux[TAMANIO][TAMANIO],int fil,int col){

 return (laberinto_aux[fil][col] == VACIO);
}

// pre: Dichos laberintos son del mismo tamaño
// pos: Copiara el laberinto original en el laberinto a mostrar
void copiar_laberinto(char laberinto_original[TAMANIO][TAMANIO], char laberinto[TAMANIO][TAMANIO]){

    for (int i = 0; i < TAMANIO; ++i){
        for (int j = 0; j < TAMANIO ; ++j){
            laberinto[i][j] = laberinto_original[i][j];
        }
    }
}

/*
 * Devolverá una coordenada aleatoria dentro del rango TAMANIOxTAMANIO.
 * No valida que dicha coordenada coincida con un pasillo ni que exista
 * otro objeto en esa posición.
 */
coordenada_t posicion_aleatoria(){

    coordenada_t coordenada_aleatoria;
    coordenada_aleatoria.fil = (rand()%TAMANIO);
    coordenada_aleatoria.col = (rand()%TAMANIO);

    return coordenada_aleatoria;
}

// pre: laberinto_aux contiene la matriz cargada con las paredes y pasillos
// pos: Devolvera una posicion aleatoria valida dentro del laberinto
coordenada_t asignar_posicion(char laberinto_aux[TAMANIO][TAMANIO]){

  coordenada_t coordenada_asignada;
  do{
      coordenada_asignada = posicion_aleatoria();
  }while(!coordenada_valida(laberinto_aux,coordenada_asignada.fil, coordenada_asignada.col));

  return coordenada_asignada;
}

// pre: laberinto_aux contiene la matriz cargada con las paredes y pasillos
// pos: Inicializara todas las estructuras relacionadas a la copa
void inicializar_copa(juego_t* juego, char laberinto_aux[TAMANIO][TAMANIO]){

    juego->copa.codigo = COPA;
    juego->copa.posicion = asignar_posicion(laberinto_aux);
    laberinto_aux[juego->copa.posicion.fil][juego->copa.posicion.col] = PARED;
}

// pre: laberinto_aux contiene la matriz cargada con las paredes y pasillos
// pos: Inicializara todas las estructuras relacionadas a los obstaculos
void inicializar_obstaculos(juego_t* juego, char laberinto_aux[TAMANIO][TAMANIO]){

    juego->tope_obstaculos = OBSTACULOS_INICIALES;

    juego->obstaculos[0].codigo = ESCREGUTO;
    juego->obstaculos[0].posicion = asignar_posicion(laberinto_aux);
    laberinto_aux[juego->obstaculos[0].posicion.fil][juego->obstaculos[0].posicion.col] = PARED;
    juego->obstaculos[0].danio = DANIO_ESCREGUTO;

    juego->obstaculos[1].codigo = ACROMANTULA;
    juego->obstaculos[1].posicion = asignar_posicion(laberinto_aux);
    laberinto_aux[juego->obstaculos[1].posicion.fil][juego->obstaculos[1].posicion.col] = PARED;
    juego->obstaculos[1].danio = DANIO_ACROMANTULA;

    juego->obstaculos[2].codigo = BOGGART;
    juego->obstaculos[2].posicion = asignar_posicion(laberinto_aux);
    laberinto_aux[juego->obstaculos[2].posicion.fil][juego->obstaculos[2].posicion.col] = PARED;
    juego->obstaculos[2].danio = DANIO_BOGGART;
}

// pre: laberinto_aux contiene la matriz cargada con las paredes y pasillos
// pos: Inicializara todas las estructuras relacionadas a las ayudas
void inicializar_ayudas(juego_t* juego, char laberinto_aux[TAMANIO][TAMANIO]){

    juego->tope_ayudas = AYUDAS_INICIALES;

    juego->ayudas[0].codigo = IMPEDIMENTA;
    juego->ayudas[0].posicion = asignar_posicion(laberinto_aux);
    laberinto_aux[juego->ayudas[0].posicion.fil][juego->ayudas[0].posicion.col] = PARED;
    juego->ayudas[0].vida_a_recuperar = VIDA_A_RECUPERAR_IMPEDIMENTA;

    juego->ayudas[1].codigo = RIDDIKULUS;
    juego->ayudas[1].posicion = asignar_posicion(laberinto_aux);
    laberinto_aux[juego->ayudas[1].posicion.fil][juego->ayudas[1].posicion.col] = PARED;
    juego->ayudas[1].vida_a_recuperar = VIDA_A_RECUPERAR_RIDDIKULUS;

    juego->ayudas[2].codigo = POCION;
    juego->ayudas[2].posicion = asignar_posicion(laberinto_aux);
    laberinto_aux[juego->ayudas[2].posicion.fil][juego->ayudas[2].posicion.col] = PARED;
    juego->ayudas[2].vida_a_recuperar = VIDA_A_RECUPERAR_POCION;

    juego->ayudas[3].codigo = POCION;
    juego->ayudas[3].posicion = asignar_posicion(laberinto_aux);
    laberinto_aux[juego->ayudas[3].posicion.fil][juego->ayudas[3].posicion.col] = PARED;
    juego->ayudas[3].vida_a_recuperar = VIDA_A_RECUPERAR_POCION;

    juego->ayudas[4].codigo = POCION;
    juego->ayudas[4].posicion = asignar_posicion(laberinto_aux);
    laberinto_aux[juego->ayudas[4].posicion.fil][juego->ayudas[4].posicion.col] = PARED;
    juego->ayudas[4].vida_a_recuperar = VIDA_A_RECUPERAR_POCION;

    juego->ayudas[5].codigo = ESFINGE;
    juego->ayudas[5].posicion = asignar_posicion(laberinto_aux);
    laberinto_aux[juego->ayudas[5].posicion.fil][juego->ayudas[5].posicion.col] = PARED;
    juego->ayudas[5].vida_a_recuperar = VIDA_A_RECUPERAR_ESFINGE;
}

// pre: laberinto_aux contiene la matriz cargada con las paredes y pasillos
// pos: Inicializara todas las estructuras relacionadas al rival
void inicializar_rival(juego_t* juego, char laberinto_aux[TAMANIO][TAMANIO]){

    do{
        juego->rival.posicion = asignar_posicion(laberinto_aux);
    }while(!distancia_valida(juego->rival.posicion,juego->copa.posicion));

    laberinto_aux[juego->rival.posicion.fil][juego->rival.posicion.col] = PARED;
    juego->rival.direccion = DERECHA;
    juego->rival.cantidad_pasos = SIN_PASOS;
    juego->rival.codigo = RIVAL;
}

// pre: laberinto_aux contiene la matriz cargada con las paredes y pasillos
// pos: Inicializara todas las estructuras relacionadas al jugador
void inicializar_jugador(juego_t* juego, char laberinto_aux[TAMANIO][TAMANIO]){

    do{
        juego->jugador.posicion = asignar_posicion(laberinto_aux);
    }while(!distancia_valida(juego->jugador.posicion,juego->copa.posicion));

    laberinto_aux[juego->jugador.posicion.fil][juego->jugador.posicion.col] = PARED;
    juego->jugador.vida = VIDA_MAXIMA;
    juego->jugador.tope_ayudas = SIN_AYUDAS;
    juego->jugador.codigo = JUGADOR;
}

/*
 * Inicializará todas las estructuras con los valores correspondientes,
 * creará el laberinto, posicionará todos los elementos, etc.
 */
void inicializar_laberinto(juego_t* juego){

    inicializar_paredes_laberinto(juego->laberinto_original);
    char laberinto_aux[TAMANIO][TAMANIO];
    copiar_laberinto(juego->laberinto_original,laberinto_aux);
    inicializar_copa(juego, laberinto_aux);
    inicializar_obstaculos(juego, laberinto_aux);
    inicializar_ayudas(juego, laberinto_aux);
    inicializar_rival(juego, laberinto_aux);
    inicializar_jugador(juego, laberinto_aux);
}

// pre: Posicion debera estar dentro del rango TAMANIOxTAMANIO
// pos: Devuelve TRUE en caso de que el jugador salga del laberinto si se mueve en la direccion indicada
bool sale_del_laberinto(char direccion, coordenada_t posicion){

  if((posicion.fil == BORDE_INFERIOR) && (direccion == ARRIBA))
    return true;

  if((posicion.fil == BORDE_MAYOR) && (direccion == ABAJO))
    return true;

  if((posicion.col == BORDE_INFERIOR) && (direccion == IZQUIERDA))
    return true;

  if((posicion.col == BORDE_MAYOR) && (direccion == DERECHA))
    return true;

  return false;
}

// pre: -
// pos: Cambiara la direccion del rival
void cambiar_direccion_rival(char* direccion){

    if((*direccion) == DERECHA)
      (*direccion) = ABAJO;

    else if((*direccion) == ABAJO)
      (*direccion) = IZQUIERDA;

    else if((*direccion) == IZQUIERDA)
      (*direccion) = ARRIBA;

    else if((*direccion) == ARRIBA)
      (*direccion) = DERECHA;

  }

// pre: posicion debe estar dentro del rango TAMANIOxTAMANIO
//      laberinto debe estar inicializado con las paredes y pasillos correspondientes
// pos: devuelve TRUE si el movimiento en la direccion indicada es valido
bool movimiento_valido(char laberinto[TAMANIO][TAMANIO], coordenada_t posicion, char direccion){
  if(!sale_del_laberinto(direccion, posicion)){

    if((direccion == DERECHA) && (laberinto[posicion.fil][posicion.col + 1] == VACIO))
      return true;

    else if((direccion == ARRIBA) && (laberinto[posicion.fil - 1][posicion.col] == VACIO))
      return true;

    else if((direccion == ABAJO) && (laberinto[posicion.fil + 1][posicion.col] == VACIO))
      return true;

    else if((direccion == IZQUIERDA) && (laberinto[posicion.fil][posicion.col - 1] == VACIO))
      return true;
    }

  return false;
}

/*
 * Determinará si el caracter ingresado es válido, esto es, es el caracter ‘a’ o
 * ‘s’ o ‘d’ o ‘w’ y además el jugador puede moverse en esa dirección, o sea,
 * hay pasillo.
 */
bool es_movimiento_valido(juego_t* juego, char tecla){

  return (movimiento_valido(juego->laberinto_original, juego->jugador.posicion, tecla));
}


//  pre: El movimiento es valido
//  pos: Movera al participante en la direccion indicada
void hacer_movimiento(coordenada_t* posicion, char direccion){

  if(direccion == ARRIBA)
    posicion->fil--;

  else if(direccion == ABAJO)
    posicion->fil++;

  else if(direccion == IZQUIERDA)
    posicion->col--;

  else if(direccion == DERECHA)
    posicion->col++;
}

/*
 * Moverá el jugador hacia la dirección especificada.
 * Dicho movimiento es válido.
 */
void mover_jugador(jugador_t* jugador, char direccion){

  hacer_movimiento(&jugador->posicion, direccion);
}

/*
 * Moverá el rival a la próxima posición.
 */
void mover_rival(juego_t* juego){

  bool se_movio = false;
  while(!se_movio){

    if(movimiento_valido(juego->laberinto_original, juego->rival.posicion, juego->rival.direccion)){

      hacer_movimiento(&juego->rival.posicion, juego->rival.direccion);
      juego->rival.cantidad_pasos++;
      se_movio = true;

      if(juego->rival.cantidad_pasos == MAX_PASOS){
        cambiar_direccion_rival(&(juego->rival.direccion));
        juego->rival.cantidad_pasos = SIN_PASOS;
      }
    }
    else{
          cambiar_direccion_rival(&(juego->rival.direccion));
          juego->rival.cantidad_pasos = SIN_PASOS;
    }
  }
}

/*
 * Actualizará el juego. Restará vida si el jugador está sobre un obstáculo
 * o lo eliminará si cuenta con el hechizo, aprenderá hechizos y todo lo
 * que pueda suceder luego de un turno.
 */
void actualizar_juego(juego_t* juego){

  modificar_vida(&(juego->jugador.vida), VIDA_POR_PASO);
  if(estado_juego((*juego)) == EN_CURSO){

    actualizar_ayudas(&(*juego));
    actualizar_obstaculos(&(*juego));
  }
}

/*
 * Devolverá el estado del juego, 1 ganado, 0 en curso, -1 perdido.
 */
int estado_juego(juego_t juego){

    if((posiciones_iguales(juego.jugador.posicion, juego.copa.posicion)) && (juego.jugador.vida > SIN_VIDA))
        return GANA_JUGADOR;

    else if((posiciones_iguales(juego.rival.posicion, juego.copa.posicion)) || (juego.jugador.vida <= SIN_VIDA))
        return GANA_RIVAL;

    else
        return EN_CURSO;
}

/*
 * Actualizará la matriz mostrada al usuario, con los elementos presentes
 * en el juego.
 */
void actualizar_laberinto(juego_t juego, char laberinto[TAMANIO][TAMANIO]){

    copiar_laberinto(juego.laberinto_original,laberinto);

    if((juego.jugador.vida < VIDA_A_MOSTRAR_COPA) || (tiene_hechizo(juego.jugador, ESFINGE)))
      laberinto[juego.copa.posicion.fil][juego.copa.posicion.col] = COPA;

    for (int i = 0; i < juego.tope_obstaculos; ++i)
      laberinto[juego.obstaculos[i].posicion.fil][juego.obstaculos[i].posicion.col] = juego.obstaculos[i].codigo;


    for (int j = 0; j < juego.tope_ayudas ; ++j)
      laberinto[juego.ayudas[j].posicion.fil][juego.ayudas[j].posicion.col] = juego.ayudas[j].codigo;


    laberinto[juego.rival.posicion.fil][juego.rival.posicion.col] = juego.rival.codigo;
    laberinto[juego.jugador.posicion.fil][juego.jugador.posicion.col] = juego.jugador.codigo;

}

/*
 * Mostrará el laberinto por pantalla.
 */
void mostrar_laberinto(char laberinto[TAMANIO][TAMANIO]){

    for (int i = 0; i < TAMANIO; ++i){
        for (int j = 0; j < TAMANIO; ++j)
            printf("%c ",laberinto[i][j]);

        printf("\n");
    }
}
