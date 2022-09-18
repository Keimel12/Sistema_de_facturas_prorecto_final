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
#define ESC 27	// Codigo ASCII que hace referencia a la tecla ESC en el teclado

void empresa();	// Muestra los datos de la empresa	
void factura_user(FILE *archivo, char code[], char name[], char city[], char rif[], char total[]);	//	Muestra en pantalla todos los elementos de la factura (falta productos)

char no_repeat(FILE *fp, char rif[]);	// Evitara ingresar un rif repetido (MISMO RIF Y NATURALEZA)
char naturaleza(char opcion);	// Devuelve la naturaleza de la persona

int admit_rif(char nationality[], char str[]);	//	Establece si un rif es admitido o no es valido
int extension(char str[]);	// Calcula la cantidad de caracteres de una cadena de palabras
int ubication_code(FILE *archivo, char str[]);	//	Busca la aparicion del elemento tecleado
int admit_naturaleza(char chr);	// Leera la inicial del char y determinara si es valido o no	

bool solo_number(char *str);	// Leera el rif (sin contar la naturaleza) y permitira continuar si solo hay numeros

// Funciones nacidas de tabdb
int isBlank_invoice(int i);	// Revisa si en el fichero de "facturas.txt" hay lineas en blanco o no
int gets_lines_invoice();	// Devuelve el total de saltos de lineas (excepto los dos ultimos) del fichero "facturas.txt"
int gets_jumplines_invoice();	// Devuelve la cantidad de lineas con contenido en el fichero "facturas.txt"

char read_ID_invoice(int i, char *str);	// Lee en la fila X el ID de la factura
char read_code_invoice(int i, char *str);	// Lee en la fila X el codigo de la factura
char read_name_invoice(int i, char *str);	// Lee en la fila X el nombre en la factura
char read_city_invoice(int i, char *str);	// Lee en la fila X la ciudad en la factura
char read_rif_invoice(int i, char *str);	// Lee en la fila X el rif en la factura
char read_date_invoice(int i, char *str);	// Lee en la fila X la fecha en la factura
char read_price_invoice(int i, char *str);	// Lee en la fila X el precio en la factura

struct STRUCTINVOICEDATA // Struct con los datos del usuario
{
	char ID[16];	// Se almacenara el ID unico de la factura
	char code[5];	//	Se almacenara numero de la factura
	char nombre[RANGO];	// Se almacenara el nombre del usuario
	char domicilio[RANGO];	// Se almacenara el domicilio del usuario
	char rif[12];	// Se almacenara el rif del usuario
	char date[12];	// Se almacenara la fecha de actual de crearse la factura
	char price[8];	// Se almacenara el precio total de los productos
} ;

struct data		//Vamos a tener todo lo refetente a un producto ordenado en esta struct 
{
	char item[40];
	char definiton[100];
	int amount;
	float price;
} product;

typedef struct STRUCTINVOICEDATA STRUCTINVOICEDATA;	

void sort_date_invoice(struct STRUCTINVOICEDATA *DATOS, int i);	// Metodo para ordenar las facturas segun la fecha
void sort_code_invoice(struct STRUCTINVOICEDATA *DATOS, int i);	// Metodo para ordenar las factura segun el codigo

int main()	
{
	FILE *archivo;	// Nombre del fichero en donde se ejecutara las operaciones
	int linea = 0;	// Almacenara el total de lineas que no este en blanco en el fochero "facturas.txt"
	int fila;	// Almacenara la fila exacta

	struct STRUCTINVOICEDATA datos;

	char str[100];	// Leera el nombre del usuario en la facturas.txt
	char aux[2]; // Volvera el origen en un string para poder usar las funciones strcat, y strcpy
	char origen; // Obtendra la naturaleza de la persona... V, E, P, J, G
	char fusion[20];	// Concatenara el origen junto al rif para tener un solo string

	int i;	// Sera usado como un contador
	int opcion;	// Segun el numero que teclees, esta variable hara una cosa o otra
	int x;	// Sera un contador al momento de recopilar el array de struct

	while(opcion != ESC){	// Mientras no presione Esc el programa seguira
	printf("1) Crear facturas\n");
	printf("2) Reporte de Facturas\n");	
	printf("3) Eliminar Factura\n");
	printf("0) Borrar el historial de facturas\n");
	printf("Presione la tecla Esc para salir\n");
	opcion = getch();
	fflush(stdin);
	
	switch(opcion)
	{
		case 49:	// CREACION DE UNA FACTURA NUEVA
			archivo = fopen("facturas.txt", "a+");
			
			linea = gets_jumplines_invoice();

			char code_number[5];	// Sera el numero de factura

			sprintf(code_number, "%.4d", linea);	// Convertimos el valor entero a un string

			printf("Dime tu nombre: \n"); fgets(datos.nombre, RANGO, stdin);
			datos.nombre[strcspn(datos.nombre, "\n")] = 0;	// Esto evitara el salto de linea de los fgets
			fflush(stdin);
		
			if(datos.nombre[0] == 0){ 	//	Puede ser cambiado por NULL
				strcpy(datos.nombre, "Undefined");
			}	

			printf("Dime tu domicilio: \n"); fgets(datos.domicilio, RANGO, stdin);
			datos.domicilio[strcspn(datos.domicilio, "\n")] = 0;
			fflush(stdin);

			if(datos.domicilio[0] == 0){ // Puede ser cambiada por NULL
				strcpy(datos.domicilio, "Undefined");
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

			char price_total[8];	// Almacenara el total en formato string
			sprintf(price_total, "%.2f", total);	// Convertimos el total en un string
			//	Mandamos el codigo de la factura(code_number), el nombre del usuario, domicilio, rif(fusion), y el precio total a una funcion para mostrar la factura
			factura_user(archivo, code_number, datos.nombre, datos.domicilio, fusion, price_total);
	
			fclose(archivo);
				break;

		case 50:	// REPORTE DE FACTURAS
			archivo = fopen("facturas.txt", "r+");
			
			if(archivo == NULL){
				printf("Error al abrir el archivo\n");
			}

			char rp[200];	// Almacenara lo escrito en una fila
			char code_2[5];	// Mostrara la factura que concuerde con el codigo unico tecleado
			char s;	//	Se usara para decidir su quiere ver la factura ampliada
			int x = 0;	// Es un contador para acceder al struct array y almacenar los datos
			i = 0;	// Es un contador
			int lines, limit;	
			
			lines = gets_lines_invoice();	// Nos da la cantidad de saltos de linea, excepto los dos ultimos	
			limit = gets_jumplines_invoice();	// Nos da la cantidad de lineas que no esten en blanco

			struct STRUCTINVOICEDATA DATOS[100];	// Convertimos nuestro struct en un array de struct para almacenar en el registro los diversos campos
			printf("Para ver todas las facturas... Teclee <1>\n");
			printf("Para las facturas segun su codigo unico... Teclee <2>\n");
			printf("Para visualizar las facturas por orden de fecha... Teclee <3>\n");
			scanf("%d", &opcion);

			while(i <= lines)	
			{
				if(!isBlank_invoice(i))	// Funcion para leer solo las lineas con contenido
				{	
					read_ID_invoice(i, DATOS[x].ID);
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

			switch(opcion)
			{	
				case 1:	// Nos muestra el total de facturas ordenados segun su codigo
					sort_code_invoice(DATOS, limit);
					for (int j = 1; j <= limit; j++)
					{
						printf("%s\t%s\t%s\t%s\t%s\t%s\t%s\n", DATOS[j].ID, DATOS[j].code, DATOS[j].nombre, DATOS[j].domicilio, DATOS[j].rif, DATOS[j].date, DATOS[j].price);
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

					fila = ubication_code(archivo, code_2);	// Buscara la fila que tenga el mismo texto que code_2 y al encontrarlo devuelve su posicion
					
					if(fila == -1)	// En caso de no hallar coincidencia nos devolvemos al menu
					{
						printf("No existe esta factura\n");
						getchar();
						getchar();
						fclose(archivo);
						break;
					}

					read_line_file(archivo, fila, rp);	// Almacena el contenido de la fila en rp
		
					system("cls");

					printf("%s\n", rp);	// Demostramos al usuario una fila con los datos

					printf("Desea ver la factura ampliada?\n");
					printf("Presione <S> para Si\n");
					printf("Presione cualquier tecla para No\n");

					printf("Escoja una opcion\n");
					fflush(stdin);
					scanf("%c", &s);	// En caso de ver los datos ampliados puede teclear s o S
						if(s == 's' || s == 'S')
						{
							char id1[16], code1[10], name[30], city[30], rif[12], date[20], total[20]; 	
							read_col_file(archivo, fila, 0, id1);	
							read_col_file(archivo, fila, 1, code1);
							read_col_file(archivo, fila, 2, name);
							read_col_file(archivo, fila, 3, city);
							read_col_file(archivo, fila, 4, rif);
							read_col_file(archivo, fila, 5, date);
							read_col_file(archivo, fila, 6, total);
							printf("--Datos de la empresa---\n");
							empresa(archivo);   
							printf("--Factura---\n");
							printf("ID: %s\n", id1);			
							printf("Codigo: %s\n", code1);
							printf("Name: %s\n", name);
							printf("City: %s\n", city);
							printf("Rif: %s\n", rif);
							printf("Date: %s\n", date);
							printf("Total: %s\n", total);
			
							getchar();
							getchar();
						} 	// En caso de no teclear s o S se ejecutara el break;
					break;
				case 3: 	//	Mostrara las facturas ordenadas por fecha (de menor a mayor)
					sort_date_invoice(DATOS, limit);	// Esta funcion se encargara de ordenar las facturas por fecha
					for(i = 0; i <= limit; i++)
					{
						printf("%s\t%s\t%s\t%s\t%s\t%s\t%s\n", DATOS[i].ID,DATOS[i].code, DATOS[i].nombre, DATOS[i].domicilio, DATOS[i].rif, DATOS[i].date, DATOS[i].price);
					}

					getchar();
					getchar();
					break;
			}
			fclose(archivo);	
			break;
		case 51: 	// ELIMINACION DE UNA FACTURA EXISTENTE
			archivo = fopen("facturas.txt", "r+");		
	
			if(archivo == NULL){
				printf("Error al abrir el archivo\n");
			}

			printf("Dime el codigo unico de la factura a eliminar\n");
			scanf("%s", code_2);
			fila = ubication_code(archivo, code_2);	// Buscara la fila cuya coincidencia sea igual que lo escrito en code_2
			
			while((fila = search_data_file(archivo, 1, code_2)) == -1)	// Si nos devuelve -1 entramos en un ciclo hasta hallarla
			{
				printf("No existe esta factura, ingrese una factura correcta:\n");
				scanf("%s", code_2);
				fila = ubication_code(archivo, code_2);
			}

			if(fila != -1)	// Si nos devuelve un valor distinto a -1 se procede a eliminar la factura
			{
				printf("Factura encontrada y eliminala\n");
				getch()
			}

			delete_line_file(archivo, fila);
			fclose(archivo);
			break;
				
		case 48:	// En caso de tener privligios de admin, aparecera esta opcion
			archivo = fopen("facturas.txt", "w");
			fprintf(archivo, "\n  \n");
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

//	Funciones  de tipo void

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

	char ID[20];
	time_t a;
    struct tm *fecha;
    char date[15];
    time(&a);

	char cont[200]; // Almacenara el total de caracteres de los campos de la factura

	fecha = localtime(&a);	
	
	create_ID(ID);	// Creamos el ID de la factura

	strftime(date, 14, "%d-%m-%Y", fecha);	// Convertimos la fecha en un string

	strcpy(cont, ID);
	strcat(cont, "\t");
	strcat(cont, code);
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

void sort_code_invoice(struct STRUCTINVOICEDATA *DATOS, int p)	// Ordenara mediante el uso del metodo burbuja los codigos de las facturas
{
	char ID[5];
	char ID2[5];

	char cod[4], cod2[4];
	STRUCTINVOICEDATA temp;

	for(int i = 1; i <= p; i++)
	{
		for(int j = 1; j <= p - i; j++)
		{
			strcpy(ID, DATOS[j].code);
			strcpy(ID2, DATOS[j + 1].code);

			substr(cod, ID, 1, 4);
			substr(cod2, ID, 1, 4);

			if(atoi(cod) < atoi(cod2))
				continue;
			else if(atoi(ID) > atoi(ID2))
			{
				temp = DATOS[j];
				DATOS[j] = DATOS[j + 1];
				DATOS[j + 1] = temp;
			}
		}
	}
}

void sort_date_invoice(struct STRUCTINVOICEDATA *DATOS, int p)	// Ordenara usando el metodo burbuja las fecha en orden descendente de las facturas
{
	char date1[11], date2[11];
	char year1[5], year2[5];
	char mes1[3], mes2[3];
	char day1[3], day2[3];

	STRUCTINVOICEDATA temp;

    for (int i = 0; i <= p; i++)
    {
        for (int j = 1; j <= p - i; j++)
        {  
        		strcpy(date1, DATOS[j].date);
				strcpy(date2, DATOS[j + 1].date);
	
				substr(day1, date1,  0, 2);
				substr(day2, date2,  0, 2);

				substr(mes1, date1,  3, 5);
				substr(mes2, date2,  3, 5);

				substr(year1, date1, 6, 10);
				substr(year2, date2,  6, 10);

				if(atoi(year1) < atoi(year2))
					continue;

				else if(atoi(year1) > atoi(year2))
				{
					temp = DATOS[j];
					DATOS[j] = DATOS[j + 1];
					DATOS[j + 1] = temp;
					continue;
				}

				else if(atoi(mes1) > atoi(mes2))
				{
					temp = DATOS[j];
					DATOS[j] = DATOS[j + 1];
					DATOS[j + 1] = temp;
					continue;
				}

				else if(atoi(day1) > atoi(day2))
				{
					temp = DATOS[j];
					DATOS[j] = DATOS[j + 1];
					DATOS[j + 1] = temp;
					continue;
				}
        }
    }		
}



//	Funciones de tipo char

char naturaleza(char opcion)	// Naturaleza de la persona 
{	
	char nat;
	switch(opcion)
	{
		case 86:
			return nat = 'V';
		break;
		case 69:
			return nat = 'E';
		break;
		case 80:
			return nat = 'P';
		break;
		case 71:
			return nat = 'G';
		break;
		case 74:
			return nat = 'J';
		break;

		default:
			fflush(stdin);
			return -1;
			break;
	}
}

// Funciones de tipo int

int admit_rif(char nationality[], char str[])	// Determina si es valido o no el rif
{	
	int digits;
	solo_number(str);
	digits = extension(str);
	if(digits == 9 && solo_number(str) == false)
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
	fila = search_data_file(fp, 1, str);
	fclose(fp);
	return fila;
}

int admit_naturaleza(char chr)	// Determinara si la naturaleza de la persona es correcta
{

	if(chr == 'V' || chr == 'E' || chr == 'G' || chr == 'J')
	{
		return 1;
	}
	return 0;
}

// Funciones de tipo bool

bool solo_number(char *str)	// Determinara que los numeros del rif no sean letras
{
	int length = lenghtStr(str);

   	char caracteresPermitidos[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '\0'};

   	int length2 = lenghtStr(caracteresPermitidos);

    for (size_t i = 0; i < length; i++)
    {
       	bool isValid = false;
            for (size_t x = 0; x < length2; x++)
            {
                if (str[i] == caracteresPermitidos[x])
                {
                   	isValid = true;
                }	
           }
       	
       	if (isValid == false)
        {
      		return true;
        }
   	}
    return false;
}

// Funciones similares pertenecientes de tabdb

int isBlank_invoice(int i)	// 
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

char read_ID_invoice(int i, char *str)	//	Devuelve el id de la facturas
{
	FILE *fp;
	fp = fopen("facturas.txt", "r+");

	if(fp == NULL){
		return -1;
	}

	read_col_file(fp, i, 0, str);

	fclose(fp);
}

char read_code_invoice(int i, char *str)	// Devuelve el codigo unico de la factura
{	
	FILE *fp;
	fp = fopen("facturas.txt", "r+");

	if(fp == NULL){
		return -1;
	}

	read_col_file(fp, i, 1, str);

	fclose(fp);
}

char read_name_invoice(int i, char *str)	// Devuelve el nombre escrito de la factura
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

char read_city_invoice(int i, char *str)	// Devuelve el domicilio escrito en la factura
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

char read_rif_invoice(int i, char *str)	// Devuelve el rif de la factura
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

char read_date_invoice(int i, char *str)	// Devuelve la fecha de impresion de la factura
{	
	FILE *fp;
	fp = fopen("facturas.txt", "r+");

	if(fp == NULL)
	{
		return -1;
	}

	read_col_file(fp, i, 5, str);

	fclose(fp);
}

char read_price_invoice(int i, char *str)	// Devuelve el precio total en la factura
{
	FILE *fp;
	fp = fopen("facturas.txt", "r");

	if(fp == NULL)
	{
		return -1;
	}

	read_col_file(fp, i, 6, str);

	fclose(fp);
}