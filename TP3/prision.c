#include "prision.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

//CONSTANTES
#define ARCHIVO_CRUCIO "crucio.dat"
#define ARCHIVO_IMPERIUS "imperius.dat"
#define ARCHIVO_LIBERADOS "liberados_"
#define EXTENSION_LIBERADOS ".txt"
#define CRUCIO_AUX "crucio_aux"
#define IMPERIUS_AUX "imperius_aux"

#define ESCRITURA "w"
#define LECTURA "r"
#define FORMATO "%[^\n]\n"

#define CANTIDAD_REGISTROS 1
#define CANTIDAD_A_LEER 1
#define MAX_LARGO_LIBERADOS 22

#define COINCIDENCIA 0
#define ERROR -1

// pre: -
// pos: Reestablece el formato estandar de la impresion por pantalla.
void reset_formato(){

  printf("\033[0m");
}

// pre: -
// pos: Establece el color para la impresion por pantalla.
void formato_color() {

  printf("\033[1;35m");
}

// pre: -
// pos: Imprime por pantalla las ayudas sobre los comandos.
void imprimir_comandos(){

	formato_color();
	printf("\e[3m" "SOBRE LOS COMANDOS: \n");
	reset_formato();

	printf("\e[3m" "\t PERDONABLES: Crea un archivo binario con todos aquellos presos que hayan hecho solo un hechizo imperdonable (Excluye Avada Kedavra)\n\n");
	printf("\t\t ARGUMENTOS: Nombre del archivo a crear\n");
	printf("\t\t    FORMATO: ./azkaban perdonables <nombre_archivo>\n\n");

	printf("\t LIBERAR: Dado el archivo de perdonados, un mes/año y una conducta minima, crea un archivo de texto con aquellos aptos para ser liberados\n\n");
	printf("\t\t ARGUMENTOS: Nombre del archivo de perdonables, año/mes de liberacion, conducta requerida\n");
	printf("\t\t    FORMATO: ./azkaban liberar <archivo_perdonables>.dat aaaamm conducta\n\n");
	
	printf("\t ACTUALIZAR: Dado un arhivo de liberados, actualizara los archivos de las maldiciones\n\n");
	printf("\t\t ARGUMENTOS: Fecha del archivo de liberados\n");
	printf("\t\t    FORMATO: ./azkaban actualizar aaaamm\n\n");
	
	printf("\t MOSTRAR LIBERADOS: Dado un arhivo de liberados, lo mostrara por pantalla\n\n");
	printf("\t\t ARGUMENTOS: Fecha del archivo de liberados\n");
	printf("\t\t    FORMATO: ./azkaban mostrar_liberados aaaamm\n\n");
}

// pre: -
// pos: imprime por pantalla las ayudas relacionadas a convenciones y usos.
void imprimir_convenciones(){

	formato_color();
	printf("SOBRE LAS CONVENCIONES:\n");
	reset_formato();

	printf("\t\e[3m" " Todas las fechas se usaran en el siguiente formato. Primero el año, luego el mes. (Expresado numericamente)\n");
	printf("\t Ejemplo: Enero de 2019 se expresa de la forma 201901\n\n");
	printf("\t La conducta se expresa con los caracteres B (Buena) - R (Regular) - M (Mala), exlusivamente en mayusculas\n\n");
	
	formato_color();
	printf("SOBRE EL CORRECTO USO DE ACTUALIZAR:\n");
	reset_formato();

	printf("\t\e[3m" " Luego de ser utilizado el comando actualizar se debera llamar al comando perdonables, y luego se podra usar correctamente el modo liberar\n");
}

// pre: nombre_archivo corresponde a un archivo que usa el programa.
// pos: muestra por pantalla una advertencia de error.
void imprimir_error(char* nombre_archivo){

	printf("Hubo un error con el archivo %s:\n", nombre_archivo);
	perror("");
}

// pre: -
// pos: imprime por pantalla el menu de ayuda para el usuario.
void mostrar_ayuda(){

	printf("Bienvenido al menu de ayudas. Aqui encontrara toda la guia que necesita para hacer uso de este programa.\n\n\n");
	
	imprimir_comandos();
	imprimir_convenciones();
}

// pre: fecha esta en el formato aaaamm.
// pos: concatena la fecha y la extension .txt luego del nombre del archivo.
void concatenar_nombre(char* fecha, char nombre_archivo[MAX_LARGO_LIBERADOS]){
	
	char* extension = EXTENSION_LIBERADOS;
	strncat(nombre_archivo,fecha,MAX_FECHA - 1);
	strcat(nombre_archivo, extension);	
}

// pre: -
// pos: devuelve TRUE si el archivo fue abierto correctamente, FALSE en caso contrario.
bool archivo_abierto(FILE* archivo){
	
	if(archivo == NULL)
		return false;
	else
		return true;
		
}

// pre: fecha es valida y esta en el formato aaaamm
// pos: devuelve TRUE si fecha_preso es previa o coincide con fecha_actual.
bool fecha_adecuada(char* fecha_preso, char* fecha_actual){
	
	int valor_fecha_preso = atoi(fecha_preso);
	int valor_fecha_actual = atoi(fecha_actual);

	return (valor_fecha_preso <= valor_fecha_actual);	
}

// pre: conducta_preso y conducta_requerida son validas.
// pos: devuelve TRUE si la conducta del preso es mejor o igual a la requerida.
bool conducta_adecuada(char conducta_preso, char* conducta_requerida){

	if((*conducta_requerida) == CONDUCTA_BUENA && conducta_preso == CONDUCTA_BUENA)
		return true;

	else if((*conducta_requerida) == CONDUCTA_REGULAR && (conducta_preso == CONDUCTA_BUENA || conducta_preso == CONDUCTA_REGULAR))
		return true;

	else if((*conducta_requerida) == CONDUCTA_MALA)
		return true;

	else
		return false;
}

// pre: preso contiene al preso a liberar, liberados ya esta abierto.
// pos: escribe al preso en el archivo liberados.
void escribir_liberado(preso_t preso, FILE* liberados){

	fprintf(liberados, "%s\n",preso.nombre);
}

// pre: liberados y perdonados ya estan abiertos, fecha esta en formato aaaamm y la conducta es valida.
// pos: determina cuales de los presos del archivo perdonados son aptos para la liberacion dada una fecha y la conducta requerida.
void determinar_liberados(FILE* perdonados, FILE* liberados, char* fecha, char* conducta_requerida){

	preso_t preso_aux;
	size_t leidos = fread(&preso_aux, sizeof(preso_t), CANTIDAD_REGISTROS, perdonados);

	while(leidos){

		if(fecha_adecuada(preso_aux.fecha, fecha) && conducta_adecuada(preso_aux.conducta, conducta_requerida))
			escribir_liberado(preso_aux, liberados);

		leidos = fread(&preso_aux, sizeof(preso_t), CANTIDAD_REGISTROS, perdonados);
	}
}

// pre: -
// pos: devuelve TRUE si existe un archivo con ese nombre, FALSE en caso contrario.
bool existe_archivo(char nombre_archivo[]){

	FILE* archivo = fopen(nombre_archivo, LECTURA);
	
	if(archivo_abierto(archivo)){
		fclose(archivo);
		return true;
	}
	else
		return false;
}

// pre: nombre_perdonados es el nombre del archivo perdonados, fecha esta en formato aaaamm y la conducta es valida.
// pos: crea un archivo con los presos a liberar en el mes indicado. Devuelve -1 si hubo un error.
int generar_archivo_liberados(char* nombre_perdonados, char* fecha, char* conducta){

	FILE* perdonados = fopen(nombre_perdonados, LECTURA);
	if(!archivo_abierto(perdonados)){
		imprimir_error(nombre_perdonados);
		return ERROR;
	}

	char nombre_archivo[MAX_LARGO_LIBERADOS] = ARCHIVO_LIBERADOS;
	concatenar_nombre(fecha, nombre_archivo);
	
	if(!existe_archivo(nombre_archivo)){

		FILE* liberados = fopen(nombre_archivo, ESCRITURA);
		if(!archivo_abierto(liberados)){
			fclose(perdonados);
			imprimir_error(nombre_archivo);
			return ERROR;
		}

		determinar_liberados(perdonados, liberados, fecha, conducta);
		fclose(liberados);
	}

	else
		printf("Ese archivo ya existe!\n");
	

	fclose(perdonados);
	return 0;
}

// pre: preso contiene la informacion de un preso valido.
// pos: escribe los datos del preso en el archivo binario.
void escribir_binario(FILE* archivo, preso_t preso){

	fwrite(&preso, sizeof(preso_t), CANTIDAD_REGISTROS, archivo);
}

// pre: crucio, imperius y perdonables ya deben estar abiertos.
// pos: determina que presos son aptos para el perdon (Esta en un solo archivo)
void determinar_perdonables(FILE* crucio, FILE* imperius, FILE* perdonables){

	preso_t preso_crucio, preso_imperius;
	size_t leidos_crucio = fread(&preso_crucio, sizeof(preso_t), CANTIDAD_REGISTROS, crucio);
	size_t leidos_imperius = fread(&preso_imperius, sizeof(preso_t), CANTIDAD_REGISTROS, imperius);
	
	while(leidos_crucio && leidos_imperius){

		int comparar = strcmp(preso_crucio.nombre, preso_imperius.nombre);

		if(comparar == COINCIDENCIA){
			leidos_crucio = fread(&preso_crucio, sizeof(preso_t),CANTIDAD_REGISTROS, crucio);
			leidos_imperius = fread(&preso_imperius, sizeof(preso_t),CANTIDAD_REGISTROS, imperius);
		}

		else if(comparar < COINCIDENCIA){	
			escribir_binario(perdonables, preso_crucio);
			leidos_crucio = fread(&preso_crucio, sizeof(preso_t),CANTIDAD_REGISTROS, crucio);	
		}

		else if(comparar > COINCIDENCIA){
			escribir_binario(perdonables, preso_imperius);
			leidos_imperius = fread(&preso_imperius, sizeof(preso_t),CANTIDAD_REGISTROS, imperius);
		}	
	}

	while(leidos_crucio){
		escribir_binario(perdonables, preso_crucio);
		leidos_crucio = fread(&preso_crucio, sizeof(preso_t),CANTIDAD_REGISTROS, crucio);
	}
	
	while(leidos_imperius){
		escribir_binario(perdonables, preso_imperius);
		leidos_imperius = fread(&preso_imperius, sizeof(preso_t),CANTIDAD_REGISTROS, imperius);		
	}
}

// pre: -
// pos: crea un archivo con el nombre indicado que contenga a los presos aptos para liberacion. Devuelve -1 si hubo error.
int generar_archivo_perdonables(char* nombre_archivo){
	
	FILE* crucio = fopen(ARCHIVO_CRUCIO, LECTURA);
	if(!archivo_abierto(crucio)){
		imprimir_error(ARCHIVO_CRUCIO);
		return ERROR;
	}

	FILE* imperius = fopen(ARCHIVO_IMPERIUS, LECTURA);
	if(!archivo_abierto(imperius)){
		imprimir_error(ARCHIVO_IMPERIUS);
		fclose(crucio);
		return ERROR;
	}

	FILE* perdonables = fopen(nombre_archivo, ESCRITURA);
	if(!archivo_abierto(perdonables)){
		imprimir_error(nombre_archivo);
		fclose(crucio);
		fclose(imperius);
		return ERROR;	
	}

	determinar_perdonables(crucio,imperius,perdonables);
	fclose(crucio);
	fclose(imperius);
	fclose(perdonables);
	return 0;
	
}

// pre: preso contiene el nombre de un preso liberado
// pos: muestra por pantalla el nombre del preso
void imprimir_liberado(preso_t preso){

	reset_formato();	
	formato_color();
	printf("Nombre: ");
	reset_formato();
	printf("\e[3m" "\e[4m" "%s\n", preso.nombre);
}

// pre: nombre_archivo es el nombre del archivo de liberados y dicho archivo existe.
// pos: muestra por pantalla todos los presos liberados del archivo nombre_archivo.
void leer_liberados(char* nombre_archivo){

	FILE* liberados = fopen(nombre_archivo, LECTURA);
	if(!archivo_abierto(liberados)){
		imprimir_error(nombre_archivo);
		return;
	}

	preso_t preso;
	int leidos = fscanf(liberados, FORMATO, preso.nombre);

	if(leidos == CANTIDAD_A_LEER){

		while(leidos == CANTIDAD_A_LEER){
			imprimir_liberado(preso);
			leidos = fscanf(liberados, FORMATO, preso.nombre);
		}
	}
	
	else{
		reset_formato();
		printf("No hay presos para este determinado mes\n");
	}
	
	
	fclose(liberados);
}

// pre: la fecha es valida.
// pos: muestra por pantalla el archivo de liberados de la fecha dada. Devuelve -1 si hubo error.
int mostrar_archivo(char* fecha_archivo){

	char nombre_archivo[MAX_LARGO_LIBERADOS] = ARCHIVO_LIBERADOS;
	concatenar_nombre(fecha_archivo, nombre_archivo);
	
	if(existe_archivo(nombre_archivo)){
		printf("\t\t\e[3m" "\e[4m" "Archivo de presos liberados para la fecha: %s\n", fecha_archivo);
		leer_liberados(nombre_archivo);
	}		

	else{
		printf("Ese archivo no existe, o no tienes acceso para leerlo!\n");
		return ERROR;
	}

	return 0;
}

// pre: aux_crucio, aux_imperius, archivo_crucio y archivo_imperius ya deben estar abiertos.
// pos: Remueve a los liberados de los archivos de las maldiciones. Devuelve -1 si hubo un error.
int remover_liberados(char* archivo_liberados, FILE* archivo_crucio, FILE* aux_crucio, FILE* archivo_imperius, FILE* aux_imperius){

	FILE* liberados = fopen(archivo_liberados, LECTURA);
	if(!archivo_abierto(liberados)){
		imprimir_error(archivo_liberados);
		return ERROR;
	}

	preso_t liberado, preso_crucio, preso_imperius;
	size_t leidos_crucio = fread(&preso_crucio, sizeof(preso_t), CANTIDAD_REGISTROS, archivo_crucio);
	size_t leidos_imperius = fread(&preso_imperius, sizeof(preso_t), CANTIDAD_REGISTROS, archivo_imperius);
	int leidos_liberados = fscanf(liberados, FORMATO, liberado.nombre);
	
	while((leidos_crucio || leidos_imperius) && (leidos_liberados == CANTIDAD_A_LEER)){

		int comparar_crucio = strcmp(preso_crucio.nombre, liberado.nombre);
		int comparar_imperius = strcmp(preso_imperius.nombre, liberado.nombre);
		
		if(comparar_crucio == COINCIDENCIA && leidos_crucio){
			leidos_crucio = fread(&preso_crucio, sizeof(preso_t), CANTIDAD_REGISTROS, archivo_crucio);
			leidos_liberados = fscanf(liberados, FORMATO, liberado.nombre);			
		}

		else if(comparar_crucio < COINCIDENCIA && leidos_crucio){
			escribir_binario(aux_crucio, preso_crucio);
			leidos_crucio = fread(&preso_crucio, sizeof(preso_t), CANTIDAD_REGISTROS, archivo_crucio);
		}

		if(comparar_imperius == COINCIDENCIA && leidos_imperius){
			leidos_imperius = fread(&preso_imperius, sizeof(preso_t), CANTIDAD_REGISTROS, archivo_imperius);
			leidos_liberados = fscanf(liberados, FORMATO, liberado.nombre);
		}

		else if(comparar_imperius < COINCIDENCIA && leidos_imperius){
			escribir_binario(aux_imperius, preso_imperius);
			leidos_imperius = fread(&preso_imperius, sizeof(preso_t), CANTIDAD_REGISTROS, archivo_imperius);
		}

		else if(comparar_crucio > COINCIDENCIA && comparar_imperius > COINCIDENCIA)
			leidos_liberados = fscanf(liberados, FORMATO, liberado.nombre);
		
	}

	while(leidos_crucio){
		escribir_binario(aux_crucio, preso_crucio);
		leidos_crucio = fread(&preso_crucio, sizeof(preso_t), CANTIDAD_REGISTROS, archivo_crucio);
	}

	while(leidos_imperius){
		escribir_binario(aux_imperius, preso_imperius);
		leidos_imperius = fread(&preso_imperius, sizeof(preso_t), CANTIDAD_REGISTROS, archivo_imperius);
	}

	fclose(liberados);
	return 0;
}

// pre: los archivos ya estan cerrados.
// pos: borra los archivos crucio e imperius originales y renombra los auxiliares a sus nombres originales.
void renombrar_archivos(){

	remove(ARCHIVO_CRUCIO);
	remove(ARCHIVO_IMPERIUS);
	
	rename(IMPERIUS_AUX, ARCHIVO_IMPERIUS);
	rename(CRUCIO_AUX, ARCHIVO_CRUCIO);
}

// pre: crucio e imperius fueron abiertos previamente.
// pos: saca a los presos liberados de los archivos crucio e imperius. Devuelve -1 si hubo un error.
int actualizar_archivos(FILE* crucio, FILE* imperius, char* nombre_archivo){
	
	FILE* crucio_aux = fopen(CRUCIO_AUX, ESCRITURA);
	if(!archivo_abierto(crucio_aux)){
		imprimir_error(CRUCIO_AUX);
		return ERROR;
	}

	FILE* imperius_aux = fopen(IMPERIUS_AUX, ESCRITURA);
	if(!archivo_abierto(imperius_aux)){
		imprimir_error(IMPERIUS_AUX);
		fclose(crucio_aux);
		return ERROR;
	}

	int retorno_liberados = remover_liberados(nombre_archivo, crucio, crucio_aux, imperius, imperius_aux);
	fclose(crucio_aux);
	fclose(imperius_aux);

	return retorno_liberados;
}

// pre: la fecha es valida.
// pos: actualizara los archivos de las maldiciones con el archivo liberados de la fecha ingresada. Devuelve -1 si hubo error.
int actualizar_maldiciones(char* fecha){
	
	char nombre_archivo[MAX_LARGO_LIBERADOS] = ARCHIVO_LIBERADOS;
	concatenar_nombre(fecha,nombre_archivo);

	if(existe_archivo(nombre_archivo)){
		
		FILE* crucio = fopen(ARCHIVO_CRUCIO, LECTURA);
		if(!archivo_abierto(crucio)){
			imprimir_error(ARCHIVO_CRUCIO);
			return ERROR;
		}

		FILE* imperius = fopen(ARCHIVO_IMPERIUS, LECTURA);
		if(!archivo_abierto(imperius)){
			imprimir_error(ARCHIVO_IMPERIUS);
			fclose(crucio);
			return ERROR;
		}

		int retorno_actualizar = actualizar_archivos(crucio, imperius, nombre_archivo);
		fclose(crucio);
		fclose(imperius);

		if(retorno_actualizar != ERROR){
			renombrar_archivos();
			return 0;
		}
		else{
			printf("Hubo un error a la hora de actualizar los archivos\n");
			return ERROR;
		}
	}

	else{
		printf("Ese archivo de liberados no existe\n");
		return ERROR;
	}
}

