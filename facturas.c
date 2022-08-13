#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX 100
#define RANGO 50

void fecha();
void user();
void empresa();
char naturaleza();
void longitud();
void exonerado();

struct usuario // Si te sientes capaz poner telefono
{
	char nombre[RANGO];
	char domicilio[RANGO];
	char i_rif;
	int rif;
} datos;

int main()
{
	int opcion;
	int code = 0;
	FILE *archivo;
	archivo = fopen("facturas.txt", "w");
	while(opcion != 5){
	printf("1) Crear facturas\n");
	printf("2) Reporte de Facturas\n");	
	printf("3) Eliminar Factura\n");
	printf("4) Modificar factura\n");
	scanf("%d", &opcion);
	code++;
		switch(opcion){
			case 1:
				fprintf(archivo, "\n------ℱ𝒶𝒸𝓉𝓊𝓇𝒶------\n");
				empresa(archivo);
				user(archivo);
				fecha(archivo);
				fprintf(archivo, "Codigo unico: %.4d\n", code);
				exonerado(archivo);
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			}
			system("cls");
		}
	


	fclose(archivo);
	return 0;
}

void empresa(FILE *archivo){
	const char rif_emp[20] = "J-123456789";
	const char name_emp[RANGO] = "Kiosco humilde";
	const char ubicacion_emp[MAX] = "Calle: Asfaltada, Estado: Bolivar, Ciudad: Puerto Ordaz";
	const char telef_emp[20] = "0414-9156515";

	fprintf(archivo, "Rif: %s\n", rif_emp);
	fprintf(archivo, "%s\n", name_emp);
	fprintf(archivo, "%s\n", ubicacion_emp);
	fprintf(archivo, "Telf: %s\n", telef_emp);
}

void fecha(FILE *archivo){
    time_t fecha = time(NULL);
    struct tm tm = *localtime(&fecha);

	fprintf(archivo, "Fecha de la factura: \n\n");
	fprintf(archivo, "%.2d-%.2d-%.4d\t\t %.2d:%.2d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
}

void user(FILE *archivo){
	char aux;
	char rif;
	int code = 0;
	fflush(stdin);
	printf("Dime tu nombre: \n"); gets(datos.nombre);
	printf("Dime tu domicilio: \n"); gets(datos.domicilio); //Extender con Calle, ciudad, y mas (usar la facturas de ejemplos)
	aux = naturaleza();
	printf("Dame el numero de tu rif: \n" ); scanf("%d", &datos.rif);
	longitud(datos.rif);
	fprintf(archivo, "\nDatos del usuario: \n\n");
	fprintf(archivo, "Nombre: %s\n", datos.nombre);
	fprintf(archivo, "Domicilio: %s\n", datos.domicilio);
	fprintf(archivo, "Rif: %c-%d\n\n", aux, datos.rif);
}

char naturaleza(){
	int opcion;
	char nat;
	printf("Eres una persona natural, teclea <1>\n");
	printf("Eres una persona natural extranjera, teclea <2>\n");
	printf("Eres una persona natural extranjera sin cedula, teclea <3>\n");
	printf("Eres un ente gubernamental, teclea <4>\n");
	printf("Eres un contribuyente juridico, teclea <5>\n");
	scanf("%d", &opcion);
	switch(opcion){
		case 1:
			return nat = *"V";
		break;
		case 2:
			return nat = *"E";
		break;
		case 3:
			return nat = *"P";
		break;
		case 4:
			return nat = *"G";
		break;
		case 5:
			return nat = *"J";
		break;

		default:
			printf("Opcion no vaalida cabron\n");
			return naturaleza();
	}
}

	void longitud(){
		bool valor;
		int aux;
		int cont = 0;
		aux = datos.rif;
		while(aux != 0){
			aux /= 10;
			cont++;
		}

		if(cont == 9){
			valor = true;
			printf("El rif es valido\n");
		}
		else if(cont < 9 || cont > 9){
			valor = false;
			printf("Rif no valido\n");
			exit(1);
		}
}

void exonerado(FILE *archivo){
	archivo = fopen("exempt", "r");
	char producto[40];
	const char exempt[700];
	fclose(archivo);
	printf("Dime el producto\n");
	for(int i = 0; i < 10; i++){
		scanf("%s", producto);
		if(strcmp(exempt, producto) == 0){
			printf("Producto exento de iva\n");
		}
	}

}