#include <stdio.h>
#include <string.h>
#include "prision.h"
#include <stdbool.h>
#include <stdlib.h>

// CONSTANTES: 

	#define ARGUMENTOS_PERDONABLES 3
	#define ARGUMENTOS_MOSTRAR 3
	#define ARGUMENTOS_LIBERAR 5
	#define ARGUMENTOS_ACTUALIZAR 3
	#define COINCIDENCIA 0
	#define LARGO_FECHA 6
	#define DIVISOR_FECHA 100
	#define MES_DICIEMBRE 12
	#define MES_ENERO 1

	#define PERDONABLES "perdonables"
	#define LIBERAR "liberar"
	#define MOSTRAR "mostrar_liberados"
	#define ACTUALIZAR "actualizar"

// PRECONDICIONES: Los archivos "crucio.dat" e "imperius.dat" siguen el formato esperado y tienen ese nombre exacto
// POSTCONDICIONES: Creara un archivo de perdonables, uno de liberados, mostrara dicho archivo de liberados y actualizara los archivos de maldiciones, siguiendo el comando del usuario. 

// pre: -
// pos: devuelve TRUE si la conducta es valida.
bool conducta_valida(char conducta){

	return (((conducta == CONDUCTA_BUENA) || (conducta == CONDUCTA_MALA) || (conducta == CONDUCTA_REGULAR)));
}

// pre: -
// pos: devuelve TRUE si la fecha es valida.
bool fecha_valida(char* fecha){

	int valor_fecha = atoi(fecha);
	int resto = (valor_fecha % DIVISOR_FECHA);

	return (((strlen(fecha) == LARGO_FECHA)) && (resto >= MES_ENERO) && (resto <= MES_DICIEMBRE));
}

int main(int argc, char *argv[]){

		if((argc == ARGUMENTOS_PERDONABLES) && (strcmp(argv[1], PERDONABLES) == COINCIDENCIA) )
			generar_archivo_perdonables(argv[2]);
		
		else if((argc == ARGUMENTOS_LIBERAR) && (strcmp(argv[1], LIBERAR) == COINCIDENCIA) && (conducta_valida(*argv[4])) && (fecha_valida(argv[3])))
			generar_archivo_liberados(argv[2],argv[3],argv[4]);

		else if((argc == ARGUMENTOS_MOSTRAR) && (strcmp(argv[1], MOSTRAR) == COINCIDENCIA) && (fecha_valida(argv[2])))
			mostrar_archivo(argv[2]);

		else if((argc == ARGUMENTOS_ACTUALIZAR) && (strcmp(argv[1], ACTUALIZAR) == COINCIDENCIA) && (fecha_valida(argv[2])))
			actualizar_maldiciones(argv[2]);

		else
			mostrar_ayuda();
	
	return 0;
}