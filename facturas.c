#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>	//BUSCAR LA BIBLIOTECA PARA PONER UN CARACTER EN MAYUSCULA PARA MAS EZ
#include "tabdb.h"

#define MAX 100
#define RANGO 30

void encabezado(FILE *fp);
void empresa();
void user(FILE *fp);
void fecha(FILE *fp);
int longitud(int num);
char extension(); //En prueba
char naturaleza();
// CREAR FUNCION PARA COMPARAR LOS RIF Y NO PERMITIR QUE SE REPITAN
void producto(FILE *fp);
// char exonerado();
void report(FILE *fp);
void ampliar(FILE *fp, int code);
void delete(FILE *fp);
void modificar(FILE *fp);

void modif_name(FILE *fp, int row, int col);
void modif_dom(FILE *fp, int row, int col);
void modif_rif(FILE *fp, int row, int col);
void modif_total(FILE *fp, int row, int col);

struct usuario // Si te sientes capaz poner telefono
{
	char nombre[RANGO];
	char domicilio[RANGO];
	char i_rif;
	int rif;
} datos;

struct data		//Vamos a tener todo lo refetente a un producto ordenado en esta struct 
{
	char item[40];
	char definiton[100];
	int amount;
	float price;
};


int main()
{

	FILE *archivo, *reemplazo;
	int opcion;
	int code = 0;

	while(opcion != 5){	
	printf("1) Crear facturas\n");
	printf("2) Reporte de Facturas\n");	
	printf("3) Eliminar Factura\n");
	printf("4) Modificar factura\n");
	printf("5) Salir\n");
	scanf("%d", &opcion);
	// BUSCAR LA FORMA QUE NO SE BUGUEE AL PONER LETRAS, 
		switch(opcion){
			case 1:	
				archivo = fopen("facturas.txt", "a+");
				code++;
				fprintf(archivo, "%.4d", code);
			//	empresa(archivo);
				user(archivo);
				fecha(archivo);
				producto(archivo);		
				fclose(archivo);	
				break;
			case 2:	
				archivo = fopen("facturas.txt", "r+");	// BUSCAR LA FORMA DE AMPLIAR LA FACTURA AMPLIADA
				
				report(archivo);

				fclose(archivo);
				break;
			case 3: 
				archivo = fopen("facturas.txt", "r+");
				
				delete(archivo);

				fclose(archivo);
				break;
			case 4: 
				archivo = fopen("facturas.txt", "r+");
				
				modificar(archivo);

				fclose(archivo);
				break;
			case 5: // VOLVER ESTO EN UN ESCAPE
				printf("Haz salido del programa\n");
				break;
			default:
				printf("Opcion no valida\n");
				getchar();
				getchar();
				break;
			}
			system("cls");
		}

	}


/* void empresa(FILE *archivo){
	const char rif_emp[20] = "J-123456789";
	const char name_emp[RANGO] = "Kiosco humilde";
	const char ubicacion_emp[MAX] = "Calle: Asfaltada, Estado: Bolivar, Ciudad: Puerto Ordaz";
	const char telef_emp[20] = "0414-9156515";

	fprintf(archivo, "Rif: %s\n", rif_emp);
	fprintf(archivo, "%s\n", name_emp);
	fprintf(archivo, "%s\n", ubicacion_emp);
	fprintf(archivo, "Telf: %s\n", telef_emp);
}	*/

void encabezado(FILE *archivo){	// DEJAR SEPARADA PARA QUE SE ENTIENDA MEJOR 
	fprintf(archivo, "Code:\t");
	fprintf(archivo, "Usuarios\t");
	fprintf(archivo, "Domicilio:\t");
	fprintf(archivo, "Rif:\t");
	fprintf(archivo, "Fecha:\t");
	fprintf(archivo, "Precio:\t\n");
}

void fecha(FILE *archivo){
    time_t fecha = time(NULL);
    struct tm tm = *localtime(&fecha);

	fprintf(archivo, "%.2d-%.2d-%.4d\t", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
	fflush(stdin);
	system("cls");
}

void user(FILE *archivo){
	char aux;
	int code = 0;
	fflush(stdin);
	printf("Dime tu nombre: \n"); gets(datos.nombre);
//	extension(datos.nombre, strlen(datos.nombre)); // APLICAR MAS LA FUNCION extension() y pos arreglarla
	printf("Dime tu domicilio: \n"); gets(datos.domicilio); //Extender con Calle, ciudad, y mas (usar la facturas de ejemplos)
	aux = naturaleza();
	printf("Dame el numero de tu rif: \n" ); 
	printf("%c-", aux); scanf("%d", &datos.rif); 
	// CREAR FUNCION PARA QUE NO SE REPITA LOS RIFS
	int digits = longitud(datos.rif);
	while(digits != 9){
		fflush(stdin);
		printf("Nota: Solamente se admiten numeros\n");
		printf("Rif invalido introduzcalo nuevamente\n");
	 	printf("%c-", aux);  scanf("%d", &datos.rif);
	 	digits = longitud(datos.rif);
	}
	fprintf(archivo, "\t");
	fprintf(archivo, "%s\t", datos.nombre);
	fprintf(archivo, "%s\t", datos.domicilio);
	fprintf(archivo, "%c-%d\t", aux, datos.rif);
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
			printf("Opcion no valida \n");
			return naturaleza();
	}
}

	int longitud(int num){
		int aux;
		int cont = 0;
		aux = num;
		while(aux != 0){
			aux /= 10;
			cont++;
		}
		return cont;
}

char extension(char line[], int exten){
	while(exten <= 31 || exten >= 6){
		printf("Introduzca un valor valido\n");
		gets(line);
		exten = strlen(line);
		if(exten < 31 || exten >= 6){
			return *line;
		}
	}
}

void producto(FILE *archivo){
	int amount, i;
	int cantidad;
	float total = 0, cont = 0;
	printf("Cantidad de producto: \n"); scanf("%d", &amount);

	fflush(stdin);		
	struct data product;

	for(i = 0; i < amount; i++){
		printf("Dime el producto: \n"); gets(product.item);	

		printf("Establece una descripcion del producto: \n"); gets(product.definiton);
		printf("Precio del producto: \n"); scanf("%f", &product.price);

		total += product.price;
		
	}	

		fprintf(archivo, "%.2f$\t\n", total);
} 


/* char exonerado(FILE *archivo, char item[]){
	char exon[800];
	int i;
	archivo = fopen("exempt.txt", "r+");	
	read_col_file(archivo, 0, 0, exon);
	if(strcmp(exon, item) == 0){
		printf("Exonerado\n");
	}
	else {
		printf("No exonerado\n");
	}	
	
	getchar();
	system("cls");
	fclose(archivo);
}	*/

void report(FILE *archivo){
	char rp[800];
	int i;
	int code;
	char s;
	printf("Para ver todas las facturas... Teclee <1>\n");	// Hacer un contador para que cada X cantidad de facturas hacer un getchar();
	printf("Para las facturas segun su codigo unico... Teclee <2>\n");
	printf("Para las facturas en un rango de tiempo... Teclee <3>\n");
	scanf("%d", &i);
	switch(i){
		case 1:
	while(fgets(rp, 100, archivo) != NULL){
		printf("%s\n", rp);
	}	
	getchar();
	getchar();
		break;
		case 2:
		printf("Dime el codigo unico de la factura\n");
		scanf("%d", &code);
		system("cls");
		read_line_file(archivo, code, rp);
		printf("%s\n", rp);

		printf("Desea ver la factura ampliada?\n");
		printf("Presione <S> para Si\n");
		printf("Presione <N> para No\n");
		while(s != 'N'){	// Probar en la mañana para que recuerdes lo que dijiste a las 01:43am
		printf("Escoja una opcion\n");
		fflush(stdin);
		scanf("%c", &s);
		if(s == 'S'){
			ampliar(archivo, code);
		}		
	}

		break;
		case 3: //A VER QUE SE ME OCURRE XD
		printf("En construccion xd\n");
		break;
	}

}

void ampliar(FILE *archivo, int code){	
	char id[10], name[30], city[30], rif[12], date[20], total[20]; 
	read_col_file(archivo, code, 0, id);
	read_col_file(archivo, code, 1, name);
	read_col_file(archivo, code, 2, city);
	read_col_file(archivo, code, 3, rif);
	read_col_file(archivo, code, 4, date);
	read_col_file(archivo, code, 5, total);
	printf("--Factura---\n");
	printf("ID: %s\n", id);
	printf("Name: %s\n", name);
	printf("City: %s\n", city);
	printf("Rif: %s\n", rif);
	printf("Date: %s\n", date);
	printf("Total: %s\n", total);
}

void delete(FILE *archivo){

	int i;
	bool dec = true;
	printf("Dime la factura que desea eliminar\n");
	scanf("%d", &i);	
	while(dec = true){
		printf("Opcion invalida\n");
		scanf("%d", &i);{
			if(i != 0){
				dec == false;
				break;
			}
		}
	}
	encabezado(archivo);
	delete_line_file(archivo, i);	
}

void modificar(FILE *archivo){
	int opcion;
	int fact;
	int col;
	printf("Que factura desea modificar?\n");
	scanf("%d", &fact);
	while(opcion != 5){
	printf("Que desea modificar de la factura?\n");
	printf("Teclee <1> para cambiar el nombre\n");
	printf("Teclee <2> para cambiar el domicilio\n");
	printf("Teclee <3> para cambiar el rif\n");
	printf("Teclee <4> para cambiar el total a pagar\n");	
	printf("Teclee <5> para salir\n");
	scanf("%d", &opcion);
	fflush(stdin);		
		if(opcion == 1){
		col = 1;
		modif_name(archivo, fact, col);
		}
		if(opcion == 2){
		col = 2;
		modif_dom(archivo, fact, col);
		}	
		if(opcion == 3){
		col = 3;
		modif_rif(archivo, fact, col);
		}
		if(opcion == 4){
		col = 5;
		modif_total(archivo, fact, col);
		}
		system("cls");	
	}

}	
	// USAR EL EJEMPLO DE MODIFICAR FICHEROS PARA EVITAR QUE SE DESCOLOQUE LOS CODE
	//ARREGLAR BUGS DE LOS MODIF_ Y BUG A PARTIR DEL ULTIMO USUARIO
void modif_name(FILE *archivo,int row, int col){	
	char new_name[RANGO];
	printf("Nuevo nombre del usuario: \n");
	gets(new_name);
	modify_col_file(archivo, row, col, new_name);
}

void modif_dom(FILE *archivo,int row, int col){
	char new_domicilio[RANGO];
	printf("Nuevo domicilio del usuario: \n");
	gets(new_domicilio);
	modify_col_file(archivo, row, col, new_domicilio);
}

void modif_rif(FILE *archivo,int row, int col){
	char new_rif[RANGO];
	printf("Nuevo rif del usuario: \n");
	gets(new_rif);
	modify_col_file(archivo, row, col, new_rif);
}

void modif_total(FILE *archivo,int row, int col){
	char new_total[RANGO];
	printf("Nuevo total a pagar: \n");
	gets(new_total);
	modify_col_file(archivo, row, col, new_total);
}