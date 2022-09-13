#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "tabdb.h"
#include "caracteres.h"
#define MAX 100
#define RANGO 32
#define ESC 27

void empresa();	// Muestra los datos de la empresa	
void factura_user(FILE *archivo, char code[], char name[], char city[], char rif[], char total[]);	//	Muestra en pantalla todos los elementos de la factura (falta productos)

char no_repeat(FILE *fp, char rif[]);	// Evitara ingresar un rif repetido (MISMO RIF Y NATURALEZA)
char naturaleza(char opcion);	// Devuelve la naturaleza de la persona
char unit_str(char str[]);	// Fusiona dos string en uno usando un '_' como un separador y evita que el usuario sobrepase la cantidad de caracteres establecidos

int admit_rif(char nationality[], char str[]);	//	Establece si un rif es admitido o no es valido
int extension(char str[]);	// Calcula la cantidad de caracteres de una cadena de palabras
int ubication_code(FILE *archivo, char str[]);	//	Busca la aparicion del elemento tecleado

// Funciones nacidas de tabdb
int isBlank_invoice(int i);
int gets_lines_invoice();
int gets_jumplines_invoice();

char read_code_invoice(int i, char *str);
char read_name_invoice(int i, char *str);
char read_city_invoice(int i, char *str);
char read_rif_invoice(int i, char *str);
char read_date_invoice(int i, char *str);
char read_price_invoice(int i, char *str);

struct STRUCTINVOICEDATA // Struct con los datos del usuario
{
	char code[5];	// Aqui obtendra el codigo unico de la factura
	char nombre[RANGO];
	char domicilio[RANGO];
	char rif[12];
	char date[12];
	char price[8];
} ;

struct data		//Vamos a tener todo lo refetente a un producto ordenado en esta struct 
{
	char item[40];
	char definiton[100];
	int amount;
	float price;
} product;


int main()	
{
	FILE *archivo;
	int linea = 0;
	int fila;	// Almacenara la fila exacta

	struct STRUCTINVOICEDATA datos;

	char str[100];	// Leera el nombre del usuario en la facturas.txt
	char aux[2]; // Volvera el origen en un string para poder usar las funciones strcat, y strcpy
	char origen; // Obtendra la naturaleza de la persona... V, E, P, J, G
	char fusion[20];	// Concatenara el origen junto al rif para tener un solo string

	int i;
	int opcion;
	int x;
	while(opcion != ESC){	// Mientras no presione Esc el programa seguira
	printf("1) Crear facturas\n");
	printf("2) Reporte de Facturas\n");	
	printf("3) Eliminar Factura\n");
	printf("4) Modificar factura\n");
	printf("0) Borrar el historial de facturas\n");	// Era 5 pero mas de una vez borre el hostorial sin querer :/
	printf("Presione la tecla Esc para salir\n");
	opcion = getch();
	fflush(stdin);
	
	switch(opcion)
	{
		case 49:	// CREACION DE UNA FACTURA NUEVA
			archivo = fopen("facturas.txt", "a+");
			
			linea = gets_jumplines_invoice();

			char ID[5];

			sprintf(ID, "%.4d", linea);

			printf("Dime tu nombre: \n"); fgets(datos.nombre, RANGO, stdin);
			datos.nombre[strcspn(datos.nombre, "\n")] = 0;	// Esto evitara el salto de linea de los fgets
			fflush(stdin);
		
			if(datos.nombre[0] == 0){	
				strcpy(datos.nombre, "Undefined");
			}	

			printf("Dime tu domicilio: \n"); fgets(datos.domicilio, RANGO, stdin);
			datos.domicilio[strcspn(datos.domicilio, "\n")] = 0;
			fflush(stdin);

			if(datos.domicilio[0] == 0){ // Puede ser cambiada por NULL
				strcpy(datos.domicilio, "Undefined");
			}

			system("cls");
			printf("Presione usando las mayusculas su opcion\n"); 
			printf("Eres una persona natural, teclea <V>\n");
			printf("Eres una persona natural extranjera, teclea <E>\n");
			printf("Eres una persona natural extranjera sin cedula, teclea <P>\n");
			printf("Eres un ente gubernamental, teclea <G>\n");
			printf("Eres un contribuyente juridico, teclea <J>\n");
			scanf("%c", &origen);

			while(naturaleza(origen) == -1)
			{
				printf("Caracter invalido\n");
				scanf("%c", &origen);
			}

			aux[1] = '\0';
			aux[0] = origen;

    		bool valor = false;	
			int digits;

			printf("Nota: Solamente se admiten numeros\n");
			printf("Dame el numero de tu rif: \n" ); 	
			fflush(stdin);	 	
	 		printf("%s-", aux);  scanf("%s", &datos.rif);

	 		strcpy(fusion, aux);
			strcat(fusion, datos.rif);

			while(admit_rif(aux, datos.rif) == -1 || no_repeat(archivo, fusion) == -1)
			{	
	 			system("cls");
	 			printf("Rif invalido, pon uno correcto\n");
	 			printf("%s-", aux);  scanf("%s", &datos.rif);
	 			strcpy(fusion, aux);
				strcat(fusion, datos.rif);
				if (no_repeat(archivo, fusion) == -1)
				{
					printf("Rif ya registrado\n");
					getchar();
					printf("Presione Enter para seguir\n");
					getchar();
				}
				printf("%s\n", fusion);
				system("cls");
			}	

			int amount, i;
			int cantidad;
			float total = 0;

			printf("Cantidad de producto: \n"); scanf("%d", &amount);

			fflush(stdin);		

			for(i = 0; i < amount; i++)
			{
				printf("Dime el producto: \n"); fgets(product.item, sizeof(product.item), stdin);	

				printf("Establece una descripcion del producto: \n"); fgets(product.definiton, sizeof(product.definiton), stdin);
				printf("Precio del producto: \n"); scanf("%f", &product.price);
				fflush(stdin);

				total += product.price;	// El total a pagar 
			}	
			// Establecer un limite del total de precio
			char price_total[8];
			sprintf(price_total, "%.2f", total);		
			factura_user(archivo, ID, datos.nombre, datos.domicilio, fusion, price_total);
	
			fclose(archivo);
				break;

		case 50:	// REPORTE DE FACTURAS
			archivo = fopen("facturas.txt", "r+");
			
			if(archivo == NULL){
				printf("Error al abrir el archivo\n");
			}

			char rp[200];
			char code_2[5];	// Mostrara la factura que concuerde con el codigo unico tecleado
			char s;
			int x = 0;
			i = 0;
			int lines, limit;

			struct STRUCTINVOICEDATA DATOS[100];
			printf("Para ver todas las facturas... Teclee <1>\n");
			printf("Para las facturas segun su codigo unico... Teclee <2>\n");
			printf("Para visualizar las facturas segun una fecha indicada... <Teclee 3>\n");
			scanf("%d", &opcion);

			switch(opcion)
			{	
				case 1:	
					lines = gets_lines_invoice();
					limit = gets_jumplines_invoice();
					while(i <= lines)
					{
						if(!isBlank_invoice(i))
						{
							read_code_invoice(i, DATOS[x].code);
							read_name_invoice(i, DATOS[x].nombre);
							read_city_invoice(i, DATOS[x].domicilio);
							read_rif_invoice(i, DATOS[x].rif);
							read_date_invoice(i, DATOS[x].date);
							read_price_invoice(i, DATOS[x].price);
							x++;		
						}
						i++;
					}	

					for (int j = 0; j <= limit; j++)
					{
						printf("%s\t%s\t%s\t%s\t%s\t%s\n", DATOS[j].code, DATOS[j].nombre, DATOS[j].domicilio, DATOS[j].rif, DATOS[j].date, DATOS[j].price);
					}
					getchar();
					getchar();
					break;
				case 2:	// Mostrar una factura en especifico y si deseas (presionando s o S) te mostrara la factura ampliada
					
					printf("Dime el codigo unico de la factura\n");
					fflush(stdin);
					scanf("%s", code_2);
					if(code_2 == ESC){
						break;
					}

					fila = ubication_code(archivo, code_2);
					read_line_file(archivo, fila, rp);	
		
					system("cls");

					printf("%s\n", rp);

					printf("Desea ver la factura ampliada?\n");
					printf("Presione <S> para Si\n");
					printf("Presione cualquier tecla para No\n");

					printf("Escoja una opcion\n");
					fflush(stdin);
					scanf("%c", &s);
						if(s == 's' || s == 'S')
						{
							char id[10], name[30], city[30], rif[12], date[20], total[20]; 
			
							read_col_file(archivo, fila, 0, id);
							read_col_file(archivo, fila, 1, name);
							read_col_file(archivo, fila, 2, city);
							read_col_file(archivo, fila, 3, rif);
							read_col_file(archivo, fila, 4, date);
							read_col_file(archivo, fila, 5, total);
							printf("--Datos de la empresa---\n");
							empresa(archivo);   
							printf("--Factura---\n");			
							printf("ID: %s\n", id);
							printf("Name: %s\n", name);
							printf("City: %s\n", city);
							printf("Rif: %s\n", rif);
							printf("Date: %s\n", date);
							printf("Total: %s\n", total);
			
							getchar();
							getchar();
						} 	
					break;
				case 3: 
					
					printf("Hacer algo aqui\n");

					break;
			}

				fclose(archivo);
				break;

		case 51: 	// ELIMINACION DE UNA FACTURA EXISTENTE
			archivo = fopen("facturas.txt", "r+");		
	
			if(archivo == NULL){
				printf("Error al abrir el archivo\n");
			}

			printf("Dime la el codigo unico de la factura a eliminar\n");
			scanf("%s", code_2);
			fila = search_data_file(archivo, 0, code_2);

			while((fila = search_data_file(archivo, 0, code_2)) == -1)
			{
				printf("No existe esta factura, ingrese una factura correcta:\n");
				scanf("%s", code_2);
				fila = ubication_code(archivo, code_2);
			}

			delete_line_file(archivo, fila);
			fclose(archivo);
			break;

		case 52: 	// MODIFICACION DE FACTURAS
			archivo = fopen("facturas.txt", "r+");
		
			if(archivo == NULL){
				printf("Error al abrir el archivo\n");
			}

			char fact[5];
			int col = 0;
			printf("Que factura desea modificar?\n");
			scanf("%s", &fact);	
			fila = ubication_code(archivo, fact);	// Creamos esta funcion para obtener la fila exacta en donde esta el code de la fila...									
			// Obviamos la posibilidad de usar el search_data_file en la fila 240 por que retorna un -1 y borra todo el fichero txt.

				while(opcion != 5)
				{
					printf("Que desea modificar de la factura?\n");
					printf("Teclee <1> para cambiar el nombre\n");
					printf("Teclee <2> para cambiar el domicilio\n");
					printf("Teclee <3> para cambiar el rif\n");
					printf("Teclee <4> para cambiar el total a pagar\n");	
					printf("Teclee <5> para salir\n");	
					scanf("%d", &opcion);		
					fflush(stdin);		
					system("cls");
 
 						if(opcion == 1)
 						{
    						char new_name[RANGO];
    						col = 1;

    						printf("Nuevo nombre del usuario: \n");
    						fgets(new_name, RANGO, stdin);
    						fflush(stdin); 

    						if(new_name[0] == 0){	// Puede ser cambiada por NULL	
								strcpy(new_name, "Undefined");
							}

							system("cls");
	
							new_name[strlen(new_name) - 1] = '\0';
							modify_col_file(archivo, fila, col, new_name);
    						break;
  						}

						if(opcion == 2)
						{
    						char new_domicilio[RANGO];
    						col = 2;

    						printf("Nuevo domicilio del usuario: \n");	
    						fgets(new_domicilio, sizeof(new_domicilio), stdin);
							fflush(stdin); 
								if(new_domicilio[0] == 0){ // Puede ser cambiada por NULL 	
									strcpy(new_domicilio, "Undefined");
								}
							system("cls");

							new_domicilio[strlen(new_domicilio) - 1] = '\0';
							modify_col_file(archivo, fila, col, new_domicilio);   
    						break;
						} 

						if(opcion == 3)
						{
							char fusion[15];
  							char new_rif[10];
  							col = 3;

  							system("cls");
							printf("Presione usando las mayusculas su opcion\n"); 
							printf("Eres una persona natural, teclea <V>\n");
							printf("Eres una persona natural extranjera, teclea <E>\n");
							printf("Eres una persona natural extranjera sin cedula, teclea <P>\n");
							printf("Eres un ente gubernamental, teclea <G>\n");
							printf("Eres un contribuyente juridico, teclea <J>\n");
							scanf("%c", &origen);

  							naturaleza(origen);

  							while(naturaleza(origen) == -1)
  							{
								printf("Caracter invalido\n");
								scanf("%c", &origen);
							}

  							aux[1] = '\0';
  							aux[0] = origen;

  							printf("Nuevo rif del usuario: \n");	

							printf("%s-", aux); scanf("%s", new_rif); 
							int digits = extension(new_rif);
							caracteresEspeciales(new_rif);

							while(admit_rif(aux, new_rif) == -1 || no_repeat(archivo, fusion) == -1)
							{	
					 			system("cls");
					 			printf("Rif invalido, pon uno correcto\n");
					 			printf("%s-", aux);  scanf("%s", &new_rif);
					 			strcpy(fusion, aux);
								strcat(fusion, new_rif);
								if (no_repeat(archivo, fusion) == -1)
								{
									printf("Rif ya registrado\n");
									getchar();
									printf("Presione Enter para seguir\n");
									getchar();
								}
								printf("%s\n", fusion);
								system("cls");
							}

							strcpy(fusion, aux);
							strcat(fusion, new_rif);

    						modify_col_file(archivo, fila, col, fusion);
    						break;
						}		

						if(opcion == 4)
						{
    						float new_total;
    						char total_price[8];
    						col = 5;

    						printf("Nuevo total a pagar: \n");
    						scanf("%f", &new_total);

    						if(new_total <= 0){
    							printf("Total invalido, ungrese un total correcto\n");
  	  							scanf("%f", &new_total);
    						}

    						snprintf(total_price, 7, "%.2f", new_total);

    						modify_col_file(archivo, fila, col, total_price);
    						break;
						}

						if(opcion == 5)
						{ // Forma temporal... o no... de arreglar el bug (El bug es, al  presionar la opcion '5' se borra toda la informacion del txt de facturas)
							break;
						}
    					system("cls");
				}

				fclose(archivo);	
				break;
				
		case 48:
			archivo = fopen("facturas.txt", "w");
			fprintf(archivo, "\n\n");
			printf("El historial ha sido borrado\n");
			fclose(archivo);
			getchar();
			break;
		case ESC:	// ESC tiene registrara el codigo ASCII de la tecla Esc del teclado por ende al presionarla se quitara el programa
			printf("Haz salido del programa\n");
			break;
		default:	// En caso de presionar una opcion que no este indicada previamente
			printf("Opcion no valida\n");
			fflush(stdin);
			getch();
			break;
		}
		system("cls");
	}
	return 0;
}

//	Funciones tipo void

void empresa(FILE *fp)	// Datos de la empresa 
{	
	const char rif_emp[20] = "J-123456789";
	const char name_emp[RANGO] = "Kiosco humilde";
	const char ubicacion_emp[MAX] = "Puerto Ordaz";
	const char telef_emp[20] = "0414-9156515";

	printf("Rif: %s\n", rif_emp);
	printf("%s\n", name_emp);
	printf("%s\n", ubicacion_emp);
	printf("Telf: %s\n", telef_emp);

}	

char no_repeat(FILE *fp, char rif[])	// No permitira que se repita el mismo rif
{	
	char cont[100];
	fp = fopen("facturas.txt", "r+");

	if(fp == NULL){
		printf("Error al abrir el archivo... (no_repeat)\n");
	}

	int fila = search_data_file(fp, 3, rif);
  	read_col_file(fp, fila, 3, cont);

  	if(!strcmp(cont, rif))
  	{
  		return -1;
  	}  

  	fclose(fp);
}

void factura_user(FILE *fp, char code[], char name[], char city[], char rif[], char total[])	// Funcion con los datos del usuario
{		
	time_t a;
    struct tm *fecha;
    char date[15];
    time(&a);

	char cont[200];

	fecha = localtime(&a);	
									
	strftime(date, 14, "%d-%m-%Y", fecha);	

	strcpy(cont, code);					
	strcat(cont, "\t");					
	strcat(cont, name);					
	strcat(cont, "\t");					
	strcat(cont, city);
	strcat(cont, "\t");
	strcat(cont, rif);
	strcat(cont, "\t");
	strcat(cont, date);
	strcat(cont, "\t");
	strcat(cont, total);
	strcat(cont, "$");
	add_line_file(fp, cont);
}

//	Funciones tipo char

char naturaleza(char opcion)	// Naturaleza de la persona 
{	
	char nat;
	switch(opcion)
	{
		case 86:
			return nat = *"V";
		break;
		case 69:
			return nat = *"E";
		break;
		case 80:
			return nat = *"P";
		break;
		case 71:
			return nat = *"G";
		break;
		case 74:
			return nat = *"J";
		break;

		default:
			fflush(stdin);
			return -1;
			break;
	}
}

char unit_str(char str[])	// Convierte los espacios en '_' y evita que el usuario sobrepase los caracteres permitidos
{	
	int j;
 	for (j = 0; str[j] != '\0'; j++) {
    	if (str[j] == ' ') str[j] = '_';
    }
	return str[j];
}

// Funciones tipo int

int admit_rif(char nationality[], char str[])	// Determina si es valido o no el rif
{	
	int digits;
	caracteresEspeciales(str);
	digits = extension(str);
	if(digits == 9 && caracteresEspeciales(str) == false)
	{
		return 0;
	}
	return -1;
}

int extension(char str[])	// Devulve el numero de caracteres de una cadena (string)
{	
    int i = 0;
    while (str[i] != '\0')
    {
  		i++;
    }
	return i;
}

int ubication_code(FILE *fp, char str[])	//	Aparicion del string
{	
	int fila;
	fp = fopen("facturas.txt","r");
	fila = search_data_file(fp, 0, str);
	fclose(fp);
	return fila;
}

// Funciones similares pertenecientes de tabdb

int isBlank_invoice(int i)
{
	FILE *fp;
	fp = fopen("facturas.txt", "r+");
	if(fp == NULL) {
		return -1;		
	}

	int space = is_blank(fp, i);
	fclose(fp);

	return space;
}

int gets_lines_invoice()
{
	FILE *fp;
	fp = fopen("facturas.txt", "r+");

	if(fp == NULL){
		return -1;
	}

	int lines = gets_lines_file(fp);

	fclose(fp);
	return lines;
}

int gets_jumplines_invoice()
{
	FILE *fp;
	fp = fopen("facturas.txt", "r+");

	if(fp == NULL){
		return -1;
	}

	int lines = gets_jumplines_file(fp);

	fclose(fp);
	return lines;
}

char read_code_invoice(int i, char *str)
{	
	FILE *fp;
	fp = fopen("facturas.txt", "r+");

	if(fp == NULL){
		return -1;
	}

	read_col_file(fp, i, 0, str);

	fclose(fp);
}

char read_name_invoice(int i, char *str)
{
	FILE *fp;
	fp = fopen("facturas.txt", "r+");

	if(fp == NULL)
	{
		return -1;
	}

	read_col_file(fp, i, 1, str);

	fclose(fp);
}

char read_city_invoice(int i, char *str)
{
	FILE *fp;
	fp = fopen("facturas.txt", "r+");

	if(fp == NULL)
	{
		return -1;
	}

	read_col_file(fp, i, 2, str);

	fclose(fp);
}

char read_rif_invoice(int i, char *str)
{
	FILE *fp;
	fp = fopen("facturas.txt", "r+");

	if(fp == NULL)
	{
		return -1;
	}

	read_col_file(fp, i, 3, str);

	fclose(fp);
}

char read_date_invoice(int i, char *str)
{	
	FILE *fp;
	fp = fopen("facturas.txt", "r+");

	if(fp == NULL)
	{
		return -1;
	}

	read_col_file(fp, i, 4, str);

	fclose(fp);
}

char read_price_invoice(int i, char *str)
{
	FILE *fp;
	fp = fopen("facturas.txt", "r");

	if(fp == NULL)
	{
		return -1;
	}

	read_col_file(fp, i, 5, str);

	fclose(fp);
}