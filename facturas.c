#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "tabdb.h"
#include "caracteres.h"
#define MAX 100
#define RANGO 30
#define ESC 27

void empresa();	// Muestra los datos de la empresa	
void factura_user(FILE *archivo, int code, char name[], char city[], char rif[]);	//Muestra en el .txt los datos del usuario y mas
void fecha(FILE *fp);	// Muestra la fecha en eñ .txt
int extension(char str[]);	// Calcula la cantidad de caracteres de una cadena de palabras
char naturaleza();	// Devuelve la naturaleza de la persona
char unit_str(char str[]);	// Fusiona dos string en uno usando un '_' como un separador
void no_repeat(FILE *fp, char rif[]);	// Evitara ingresar un rif repetido (MISMO RIF Y NATURALEZA)
void ampliar(FILE *fp, int code);	// Ampliara la factura (En vez de visualizarla en fila sera en columna)
int admit_rif(char nationality[], char str[]);	//	Establece si un rif es admitido o no es valido
int ubication_code(FILE *archivo, char str[]);	//	Busca la aparicion del elemento tecleado


// Los 4 aspectos que permitira modificar la factura: 
void modify_name(FILE *origen, FILE *destino, char new_name[], int i);
void modify_city(FILE *origen, FILE *destino, char new_domicilio[], int fila);
void modify_rif(FILE *origen, FILE *destino, char aux[], char new_rif[], int i);
void modify_mount(FILE *origen, FILE *destino, float new_total, int i);

struct usuario // Struct con los datos del usuario
{
	char nombre[RANGO];
	char domicilio[RANGO];
	char i_rif;
	char rif[10];
} datos;

struct data		//Vamos a tener todo lo refetente a un producto ordenado en esta struct 
{
	char item[40];
	char definiton[100];
	int amount;
	float price;
} product;


int main()	
{
	FILE *archivo, *reemplazo;

	int code = 0;	// Variable que sera el codigo unico de la factura... que se incrementara en uno cada vez que crearemos una nueva factura
	char str[30];	// Leera el nombre del usuario en la facturas.txt
	char str_aux[30];	//Leera el domicilio de usuario en la facturas.txt
	char rif_num[20];	// Leera el rif del usuario en la facturas.txt
	char date[20];	// Leera la fecha de creacion de la factura en facturas.txt
	float foat;	// Leera el monto total a pagar en facturas.txt

	char aux[2]; // Volvera el origen en un string para poder usar las funciones strcat, y strcpy
	char origen; // Obtendra la naturaleza de la persona... V, E, P, J, G
	char fusion[20];	// Concatenara el origen junto al rif para tener un solo string

	int opcion;

	while(opcion != ESC){	// Mientras no presione Esc el programa seguira
	printf("1) Crear facturas\n");
	printf("2) Reporte de Facturas\n");	
	printf("3) Eliminar Factura\n");
	printf("4) Modificar factura\n");
	printf("Presione la tecla Esc para salir\n");
	opcion = getch();
		switch(opcion){
			case 49:	// CREACION DE UNA FACTURA NUEVA
			archivo = fopen("facturas.txt", "a+");

	while (!feof(archivo)) {	// No permitira que se repita el mismo numero de codigo en el .txt
    	fscanf(archivo, "%d %s %s %s %s %f\n", &code, str, str_aux, rif_num, date, &foat);        				
        code++;
	}

	fflush(stdin);
	printf("Dime tu nombre: \n"); gets(datos.nombre);
	
	if(datos.nombre[0] == NULL){	// Si el usuario no teclea nada (solamente presiona enter) mostrara el texto Undefined en la facturas.txt
		strcpy(datos.nombre, "Undefined");
	}
 		
	unit_str(datos.nombre);

	printf("Dime tu domicilio: \n"); gets(datos.domicilio);
	
	if(datos.domicilio[0] == NULL){
		strcpy(datos.domicilio, "Undefined");
	}	

	unit_str(datos.domicilio);

	origen = naturaleza();

	aux[1] = '\0';
	aux[0] = origen;

    bool valor = false;	
	int digits;

		printf("Nota: Solamente se admiten numeros\n");
		printf("Dame el numero de tu rif: \n" ); 	
		fflush(stdin);	 	
	 	printf("%s-", aux);  scanf("%s", &datos.rif);
		while(admit_rif(aux, datos.rif) == -1){	
			printf("Rif invalido, pon uno correcto\n");
	 		system("cls");
	 		printf("%s-", aux);  scanf("%s", &datos.rif);
	}	

	strcpy(fusion, aux);
	strcat(fusion, datos.rif);

	no_repeat(archivo, fusion);  // Pensar una mejor forma que aventar al usuario fuera del programa XD
	factura_user(archivo, code, datos.nombre, datos.domicilio, fusion);	

	fecha(archivo);		

	int amount, i;
	int cantidad;
	float total = 0;

	printf("Cantidad de producto: \n"); scanf("%d", &amount);

	fflush(stdin);		

	for(i = 0; i < amount; i++){
		printf("Dime el producto: \n"); gets(product.item);	

		printf("Establece una descripcion del producto: \n"); gets(product.definiton);
		printf("Precio del producto: \n"); scanf("%f", &product.price);
		fflush(stdin);

		total += product.price;	// El total a pagar 
	}	

	fprintf(archivo, "%.2f\t\n", total);
	
	fclose(archivo);

			break;
			case 50:	// REPORTE DE FACTURAS
			archivo = fopen("facturas.txt", "r+");
			
			if(archivo == NULL){
				printf("Error al abrir el archivo\n");
			}

	char rp[200];
	char code_2[5];
	char s;

	printf("Para ver todas las facturas... Teclee <1>\n");
	printf("Para las facturas segun su codigo unico... Teclee <2>\n");
	scanf("%d", &opcion);
	switch(opcion){	
		case 1:	
	while(fgets(rp, 100, archivo) != NULL){	
		printf("%s\n", rp);					
	}										
	getchar();								
	getchar();								
		break;
		case 2:	// Mostrar una factura en especifico y si deseas (presionando s o S) te mostrara la factura ampliada
		printf("Dime el codigo unico de la factura\n");
		fflush(stdin);
		scanf("%s", code_2);

		int fila;
		fila = search_data_file(archivo, 0, code_2);
		read_line_file(archivo, fila, rp);	

		system("cls");

		printf("%s\n", rp);

		printf("Desea ver la factura ampliada?\n");
		printf("Presione <S> para Si\n");
		printf("Presione cualquier tecla para No\n");

		printf("Escoja una opcion\n");
		fflush(stdin);
		scanf("%c", &s);
		if(s == 's' || s == 'S'){
			ampliar(archivo, fila);
			getchar();
			getchar();
		} 
		else {
			break;
		}	

		break;
	}

			fclose(archivo);
			break;

			case 51: 	// ELIMINACION DE UNA FACTURA EXISTENTE
		archivo = fopen("facturas.txt", "r+");		
		reemplazo = fopen("temporal", "w+");
	
		if(archivo == NULL || reemplazo == NULL){
			printf("Error al abrir el archivo\n");
		}

	int id;	// Sera el codigo unico de la factura
	int linea;

	printf("Dime la factura que desea eliminar\n");
	fflush(stdin);
	scanf("%d", &id);	
	linea = 0;

        while (!feof(archivo))
        {
        linea++;
        fscanf(archivo, "%d %s %s %s %s %f\n", &code, str, str_aux, rif_num, date, &foat);
            if (linea != id) // Se borra el valor tecleado en la i (i debe ser el codigo de la factura)
        	fprintf(reemplazo, "%.4d\t%s\t%s\t%s\t%s\t%.2f\n", code, str, str_aux, rif_num, date, foat);
    	}

    fclose(archivo);
    fclose(reemplazo);
    remove("facturas.txt");
    rename("temporal", "facturas.txt");

			fclose(archivo);
			break;

			case 52: 	// MODIFICACION DE FACTURAS
		archivo = fopen("facturas.txt", "r+");
		reemplazo = fopen("temporal", "w+");
		
		if(archivo == NULL || reemplazo == NULL){
			printf("Error al abrir el archivo\n");
		}

	char fact[5];
    int fila;	
	printf("Que factura desea modificar?\n");
	scanf("%s", &fact);	
	fila = ubication_code(archivo, fact) + 1;	// Creamos esta funcion para obtener la fila exacta en donde esta el code de la fila...									
// Obviamos la posibilidad de usar el search_data_file en la fila 240 por que retorna un -1 y borra todo el fichero txt.

	while(opcion != 5){

	printf("Que desea modificar de la factura?\n");
	printf("Teclee <1> para cambiar el nombre\n");
	printf("Teclee <2> para cambiar el domicilio\n");
	printf("Teclee <3> para cambiar el rif\n");
	printf("Teclee <4> para cambiar el total a pagar\n");	
	printf("Teclee <5> para salir\n");	
	scanf("%d", &opcion);		
	fflush(stdin);		
	system("cls");
 
 if(opcion == 1){

    char new_name[RANGO];

    printf("Nuevo nombre del usuario: \n");
    fflush(stdin); 
    gets(new_name);

    if(new_name[0] == NULL){
		strcpy(new_name, "Undefined");
	}

	unit_str(new_name);

    modify_name(archivo, reemplazo, new_name, fila);	// Funcion para cambiar el nombre
    break;
  }

if(opcion == 2){
    char new_domicilio[RANGO];

    printf("Nuevo domicilio del usuario: \n");
    fflush(stdin); 
    gets(new_domicilio);
	
    if(new_domicilio[0] == NULL){
		strcpy(new_domicilio, "Undefined");
	}

	unit_str(new_domicilio);
    
    modify_city(archivo, reemplazo, new_domicilio, fila);	// Funcion para cambiar el domicilio
    break;
	} 

if(opcion == 3){

  char new_rif[10];

  origen = naturaleza();

  aux[1] = '\0';
  aux[0] = origen;

  printf("Nuevo rif del usuario: \n");	

	printf("%s-", aux); scanf("%s", new_rif); 
	int digits = extension(new_rif);
	caracteresEspeciales(new_rif);

    modify_rif(archivo, reemplazo, aux, new_rif, fila);	// Funcion para cambiar el rif
    break;
	}		

if(opcion == 4){
    float new_total;
    printf("Nuevo total a pagar: \n");
    scanf("%f", &new_total);
    modify_mount(archivo, reemplazo, new_total, fila);	// Funcion para cambia el total
    break;
    }

if(opcion == 5){ // Forma temporal... o no... de arreglar el bug (El bug es, al  presionar la opcion '5' se borra toda la informacion del txt de facturas)
	return 0; 
	}

    	system("cls");
	}

	fclose(archivo);	
    fclose(reemplazo);
    remove("facturas.txt");
    rename("temporal", "facturas.txt");

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

	}

int ubication_code(FILE *archivo, char str[]){	//	Aparicion del string
	int fila;
	archivo = fopen("facturas.txt","r");
	fila = search_data_file(archivo, 0, str);
	fclose(archivo);
	return fila;
}


void empresa(FILE *archivo){	// Datos de la empresa x2
	const char rif_emp[20] = "J-123456789";
	const char name_emp[RANGO] = "Kiosco humilde";
	const char ubicacion_emp[MAX] = "Puerto Ordaz";
	const char telef_emp[20] = "0414-9156515";

	printf("Rif: %s\n", rif_emp);
	printf("%s\n", name_emp);
	printf("%s\n", ubicacion_emp);
	printf("Telf: %s\n", telef_emp);

}	

void fecha(FILE *archivo){	// Horario local 
    time_t fecha = time(NULL);
    struct tm tm = *localtime(&fecha);

	fprintf(archivo, "%.2d-%.2d-%.4d\t", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
	fflush(stdin);
	system("cls");
}

int admit_rif(char nationality[], char str[]){	// Determina si es valido o no el rif
	 	int digits;
	 	caracteresEspeciales(str);
	 	digits = extension(str);
	 	if(digits == 9 && caracteresEspeciales(str) == false){
	 		return 0;
	 	}	else 	{
	 		return -1;
	 	}
	 	return -1;
}

void no_repeat(FILE *archivo, char rif[]){	// No permitira que se repita el mismo rif
	char cont[100];
	archivo = fopen("facturas.txt", "r");

	if(archivo == NULL){
		printf("Error al abrir el archivo\n");
	}

	int fila = search_data_file(archivo, 3, rif);
  	read_col_file(archivo, fila, 3, cont);

  if(!strcmp(cont, rif)){
    printf("El rif ya esta registrado\n");
   	exit(1);
  }  
  fclose(archivo);
}

void factura_user(FILE *archivo, int code, char name[], char city[], char rif[]){	// Funcion con los datos del usuario
	char cont[200];
	char aux[10];
										//
	snprintf(aux, 5, "%.4d", code);		//
										//
	strcpy(cont, aux);					// Pensar a fondo si implementar el monto total del producto y la fecha tambien
	strcat(cont, "\t");					//
	strcat(cont, name);					//
	strcat(cont, "\t");					//
	strcat(cont, city);
	strcat(cont, "\t");
	strcat(cont, rif);
	strcat(cont, "\t");

	add_line_file(archivo, cont);

}

char unit_str(char str[]){	// Convierte los espacios en '_' y pondra Undefined si el usuario le da enter de una
	int j;
 	for (j = 0; str[j] != '\0'; j++) {
    	if (str[j] == ' ') str[j] = '_';
    }

	return str[j];
}


char naturaleza(){	// Naturaleza de la persona 
	char opcion;
	char nat;
	printf("Presione usando las mayusculas su opcion\n"); // 01:46am.... Despues pensare en como admitir minusculas xd
	printf("Eres una persona natural, teclea <V>\n");
	printf("Eres una persona natural extranjera, teclea <E>\n");
	printf("Eres una persona natural extranjera sin cedula, teclea <P>\n");
	printf("Eres un ente gubernamental, teclea <G>\n");
	printf("Eres un contribuyente juridico, teclea <J>\n");
	scanf("%c", &opcion);
	switch(opcion){
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
			printf("Opcion no valida \n");
			fflush(stdin);
			return naturaleza();
	}
}


void ampliar(FILE *archivo, int fila){	// De aqui saldra la forma ampliada que visualizaras de la factura
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
}


int extension(char str[]){	// Devulve el numero de caracteres de una cadena (string)
        int i = 0;
        while (str[i] != '\0')
        {
                i++;
        }
        return i;
}

// Funciones para modificar las facturas

void modify_name(FILE *archivo, FILE *reemplazo, char new_name[], int i){

    int code; 
    char str[30];
    char str_aux[30];
    char rif_num[20];
    char date[20];
    float foat;
    int linea; 

    linea = 0;        

        while (!feof(archivo))
        {
        linea++;
          fscanf(archivo, "%d %s %s %s %s %f\n", &code, str, str_aux, rif_num, date, &foat);
            if (linea == i) 
            fprintf(reemplazo, "%.4d\t%s\t%s\t%s\t%s\t%.2f\n", code, new_name, str_aux, rif_num, date, foat);
                         
            else   
          	fprintf(reemplazo, "%.4d\t%s\t%s\t%s\t%s\t%.2f\n", code, str, str_aux, rif_num, date, foat); 
        }
}

void modify_city(FILE *archivo, FILE *reemplazo, char new_domicilio[], int fila){

    int code; 
    char str[30];
    char str_aux[30];
    char rif_num[20];
    char date[20];
    float foat;
    int linea; 

    linea = 0; 

      while (!feof(archivo))
        {
        linea++;
        fscanf(archivo, "%d %s %s %s %s %f\n", &code, str, str_aux, rif_num, date, &foat);
            if (linea == fila)             
			fprintf(reemplazo, "%.4d\t%s\t%s\t%s\t%s\t%.2f\n", code, str, new_domicilio, rif_num, date, foat);
                         
            else   
			fprintf(reemplazo, "%.4d\t%s\t%s\t%s\t%s\t%.2f\n", code, str, str_aux, rif_num, date, foat);
      }
}

void modify_rif(FILE *archivo, FILE *reemplazo, char aux[] , char new_rif[], int i){
  
    int code; 
    char str[30];
    char str_aux[30];
    char rif_num[20];
    char date[20];
    float foat;
    int linea; 
    char fusion[20];
    bool valor = false;

  	int digits = extension(new_rif);
 	caracteresEspeciales(new_rif);

	while(valor == false){
		valor = false;			
		printf("Nota: Solamente se admiten numeros\n");
		printf("Dame el numero de tu rif: \n" ); 	
		fflush(stdin);
	 	printf("%s-", aux);  scanf("%s", new_rif);
	 	caracteresEspeciales(new_rif);
	 	digits = extension(new_rif);
	 	if(digits == 9 && caracteresEspeciales(new_rif) == false){
	 		valor = true;
	 	}	else 	{
	 		valor = false;
	 	}
	 	printf("Rif invalido introduzcalo nuevamente\n");
	 	getchar();
	 	system("cls");
	}	

	strcpy(fusion, aux);
	strcat(fusion, new_rif); 

	no_repeat(archivo, fusion);  // Pensar una mejor forma que aventar al usuario fuera del programa XD
    linea = 0;

        while (!feof(archivo))
        {
        linea++;
        fscanf(archivo, "%d %s %s %s %s %f\n", &code, str, str_aux, rif_num, date, &foat);
            if (linea == i)              
			fprintf(reemplazo, "%.4d\t%s\t%s\t%s\t%s\t%.2f\n", code, str, str_aux, fusion, date, foat); 

            else   
			fprintf(reemplazo, "%.4d\t%s\t%s\t%s\t%s\t%.2f\n", code, str, str_aux, rif_num, date, foat);
        } 
}

void modify_mount(FILE *archivo, FILE *reemplazo, float new_total, int i){
    int code; 
    char str[30];
    char str_aux[30];
    char rif_num[20];
    char date[20];
    float foat;
    int linea; 

    linea = 0;

    while (!feof(archivo))
        {
        linea++;
        fscanf(archivo, "%d %s %s %s %s %f\n", &code, str, str_aux, rif_num, date, &foat);
            if (linea == i)              
			fprintf(reemplazo, "%.4d\t%s\t%s\t%s\t%s\t%.2f\n", code, str, str_aux, rif_num, date, new_total);
                         
            else   
			fprintf(reemplazo, "%.4d\t%s\t%s\t%s\t%s\t%.2f\n", code, str, str_aux, rif_num, date, foat);
        }
}