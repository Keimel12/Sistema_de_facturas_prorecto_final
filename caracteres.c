#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <conio.h>
#include <string.h>
#include "caracteres.h"
#include "tabdb.h"

bool caracteresEspeciales(char *str)
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

char *subString(const char *str, int from, int to)
{
        int length = to - from;
        char *dest = (char *)malloc(sizeof(char) * (length + 1));
        int x = 0;
        for (int i = from; i <= to; i++)
        {
                dest[x] = str[i];
                x++;
        }
        dest[length] = '\0';
        return dest;
}




int select(int len, char *enunciado, ...)
{

        if (enunciado != NULL)
                puts(enunciado);

        va_list ap;

        int posicion = 0;
        int run = 1;

        while (run)
        {
                va_start(ap, enunciado);


                if (enunciado != NULL)
                        puts(enunciado);

                for (int i = 0; i < len; i++)
                {
                        char *texto = va_arg(ap, char *);
                        if (posicion == i)
                        {
                                printf("  %s%s  ", texto);
                        }
                        else
                                printf("%s\n", texto);
                }

                int accion = getch();
                switch (accion)
                {
                case 72: // Flecha de arriba
                        if (posicion > 0)
                                posicion--;
                        break;

                case 80: // Flecha de abajo
                        if (posicion < len - 1)
                                posicion++;
                        break;

                case 27: // ESC
                        run = 0;
                        break;

                case 13: // ENTER
                        va_end(ap);
                        return posicion;
                        break;
                }
                va_end(ap);
        }
        return -1;
}