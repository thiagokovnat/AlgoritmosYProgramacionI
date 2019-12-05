#ifndef __PRISION_H__
#define __PRISION_H__

#define MAX_FECHA 7
#define MAX_NOMBRE 200
#define CONDUCTA_BUENA 'B'
#define CONDUCTA_REGULAR 'R'
#define CONDUCTA_MALA 'M'

typedef struct preso{
	char nombre[MAX_NOMBRE];
	unsigned int edad;
	char conducta;
	unsigned int pabellon;
	unsigned int celda;
	char fecha[MAX_FECHA]; 
	int maldicion_realizada; 
}preso_t;

// pre: -
// pos: imprime por pantalla el menu de ayuda para el usuario.
void mostrar_ayuda();

// pre: -
// pos: crea un archivo con el nombre indicado que contenga a los presos aptos para liberacion.
int generar_archivo_perdonables(char* nombre_archivo);

// pre: nombre_perdonados existe y es el nombre del archivo perdonados, fecha esta en formato aaaamm y la conducta es valida.
// pos: crea un archivo con los presos a liberar en el mes indicado.
int generar_archivo_liberados(char* nombre_liberados, char* fecha, char* conducta);

// pre: la fecha es valida.
// pos: muestra por pantalla el archivo de liberados de la fecha dada.
int mostrar_archivo(char* fecha_archivo);

// pre: la fecha es valida.
// pos: actualizara los archivos de las maldiciones con el archivo liberados de la fecha ingresada.
int actualizar_maldiciones(char* fecha);



#endif /* __PRISION_H__ */