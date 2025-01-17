#include <stdio.h>
#include "funciones.h"

int main (int argc, char *argv[])
{
    int opc=0, opc2=0;
    do
    {
        opc = menu();
        switch (opc)
        {
        case 1:
            createFactura();
            break;
        case 2:
            readFacturas();
            break;
        case 3:
            printf("Saliendo...\n");
            break;
        default:
            printf("Opcion no valida\n");
            break;
        }
        printf("Desea realizar otra operacion? 1.Si/2.No: ");
        scanf("%d", &opc2);
    } while (opc2 != 2);
    return 0;
}