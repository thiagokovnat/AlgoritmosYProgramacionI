#include <stdio.h>
#include <stdbool.h>
/*
PRECONDICIONES:
- EL USUARIO DEBERA INGRESAR EL TIPO DE DATO QUE CORRESPONDA PARA EL DATO QUE SE PIDE
- EL USUARIO, AL INGRESAR CARACTERES, SOLO DEBE HACERLO EN MAYUSCULAS

POSCONDICIONES:
- EL PROGRAMA IMPRIMIRA POR PANTALLA UN UNICO CAMPEON O, SI FUESE EL CASO, INFORMARA DE LA AUSENCIA DE UNO 
*/
const unsigned int EDAD_MINIMA = 17;
const unsigned int INTELIGENCIA_MINIMA = 0;
const unsigned int INTELIGENCIA_MAXIMA = 10;
const unsigned int FUERZA_MINIMA = 0;
const unsigned int FUERZA_MAXIMA = 10;

void inicializacion_caliz(char* iniciar_caliz){

	printf("Desea ingresar un alumno para comenzar el proceso de seleccion? (N para NO, cualquier otra tecla para SI)\n");
	scanf(" %c",iniciar_caliz);
}
int valoracion_capacidad_magica(char capacidad_magica){  
	
    if (capacidad_magica == 'B')
		return 3;

	else if (capacidad_magica == 'R')
		return 2;

	else if (capacidad_magica == 'M')
		return 1;

}
void ingresar_otro_alumno(char* ingreso_otro_alumno){

	printf("¿Desea ingresar otro alumno? (N para NO, cualquier otra tecla para SI)\n");
	scanf(" %c", ingreso_otro_alumno);

}

void imprimir_campeon_actual(int numero_campeon_actual){

	if (numero_campeon_actual != 0)
		printf("\nEl campeón es el número %i :)\n", numero_campeon_actual);
	else
		printf("No hay campeón :(\n");

}

bool evaluar_ingreso_otro_alumno(char ingreso_otro_alumno){
	return (ingreso_otro_alumno != 'N');
}

bool capacidad_magica_valida (char capacidad_magica){
	return (capacidad_magica =='B' || capacidad_magica =='R' || capacidad_magica=='M');
}

bool inteligencia_valida(short inteligencia){
	return (inteligencia>=INTELIGENCIA_MINIMA && inteligencia<=INTELIGENCIA_MAXIMA);
}

bool fuerza_valida(short fuerza){
	return (fuerza>=FUERZA_MINIMA && fuerza<=FUERZA_MAXIMA);
}

void pedir_datos_alumno(unsigned int* edad,short* inteligencia,short* fuerza, char* capacidad_magica){
	
    printf("Ingrese los siguientes datos del alumno:\n");
	printf("Edad:\n");
	scanf("%u",edad);
	if ((*edad)>=EDAD_MINIMA){
			do{
	 			printf("Inteligencia (Unicamente valores del 1 al 10):\n");
	 			scanf("%hi",inteligencia);
	 	  	  }	while (!inteligencia_valida(*inteligencia));
	 	  	 do{
	 			printf("Fuerza (Unicamente valores del 1 al 10):\n");
	 			scanf("%hi",fuerza);
	 	  	  }	while (!fuerza_valida(*fuerza));
			 do{
			 	printf("Capacidad Magica (B para BIEN, R para REGULAR, M para MAL): \n");
	 	  	  	scanf(" %c", capacidad_magica);
			 } while (!capacidad_magica_valida(*capacidad_magica));  	  
	 }	
}
bool evaluar_si_es_campeon(short inteligencia, short fuerza, int valor_capacidad_alumno, short inteligencia_campeon, short fuerza_campeon, int valor_capacidad_campeon){  

		if (inteligencia>inteligencia_campeon)
			return true;

		else if (fuerza>fuerza_campeon && inteligencia_campeon == inteligencia)
		 	return true;

		else if (valor_capacidad_alumno>valor_capacidad_campeon && fuerza_campeon == fuerza && inteligencia == inteligencia_campeon)
		 	return true;

		else
		 	return false;
}

int main(){

	unsigned int edad = 0;
	short inteligencia = 0;
	short fuerza = 0;
	char capacidad_magica;
	int valor_capacidad_alumno = 0;
	int valor_capacidad_campeon = 0;
	short inteligencia_campeon = 0;
	short fuerza_campeon = 0;
	char ingreso_otro_alumno;
	int numero_alumno_ingresado = 0;
	int numero_campeon_actual = 0;
	char iniciar_caliz;

	inicializacion_caliz(&iniciar_caliz);
	if(iniciar_caliz != 'N'){
 		do{
 			numero_alumno_ingresado++;
			pedir_datos_alumno(&edad,&inteligencia,&fuerza,&capacidad_magica);
			valor_capacidad_alumno = valoracion_capacidad_magica(capacidad_magica);
	
			if (evaluar_si_es_campeon(inteligencia,fuerza,valor_capacidad_alumno,inteligencia_campeon,fuerza_campeon,valor_capacidad_campeon)){   
				
				inteligencia_campeon = inteligencia;
				fuerza_campeon = fuerza;
				valor_capacidad_campeon = valor_capacidad_alumno;
				numero_campeon_actual=numero_alumno_ingresado;
			}
			
			ingresar_otro_alumno(&ingreso_otro_alumno);

		} while(evaluar_ingreso_otro_alumno(ingreso_otro_alumno));	
		imprimir_campeon_actual(numero_campeon_actual);
	} 		
	else
		printf("No hay campeón :(\n");

	return 0;
}
