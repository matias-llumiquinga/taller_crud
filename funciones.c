#include <stdio.h>
#include <string.h>
#include "funciones.h"


int menu()
{
    int opc;
    printf("1. Crear factura\n");
    printf("2. Ver facturas\n");
    printf("3. Salir\n");
    printf("Ingrese una opcion: ");
    scanf("%d", &opc);
    return opc;
}

void save(struct Factura *factura)
{
    FILE *file = fopen("factura.data", "ab+");
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }
    else
    {
        fwrite(factura, sizeof(struct Factura), 1, file); // putero de estructura, tamaño de la estructura, cantidad de estructuras, archivo
        printf("Factura guardada con exito\n");
    }

    fclose(file);
}

void leerCadena(char *cadena, int tam)
{
    while (getchar() != '\n');
    fgets(cadena, tam, stdin);
    int len = strlen(cadena) - 1;
    cadena[len] = '\0';
}

void createFactura()
{
    struct Factura factura;
    printf("Ingrese el nombre del cliente: ");
    leerCadena(factura.nombre, 20);
    printf("Ingrese la cedula del cliente: ");
    scanf("%f", &factura.cedula);
    printf("Ingrese la cantidad de productos: ");
    scanf("%d", &factura.cantidadProductos);
    factura.total = 0;
    for (int i = 0; i < factura.cantidadProductos; i++)
    {
        printf("Ingrese el nombre del producto %d: ", i + 1);
        leerCadena(factura.productos[i].nombre, 50);
        printf("Ingrese la cantidad del producto %d: ", i + 1);
        scanf("%d", &factura.productos[i].cantidad);
        printf("Ingrese el precio del producto %d: ", i + 1);
        scanf("%f", &factura.productos[i].precio);
        factura.total += factura.productos[i].cantidad * factura.productos[i].precio;
    }
    save(&factura);
}

void imprimirFacturas(struct Factura factura)
{
    printf("%d\t\t%s\t\t%.2f\n", factura.cedula, factura.nombre, factura.total);
}

void imprimirDeatalleFactura(struct Factura factura)
{
    printf("Cedula: %d\n", factura.cedula);
    printf("Nombre: %s\n", factura.nombre);
    printf("Nombre\t\tCantidad\t\tPrecio\n");

    printf("Nombre\t\tCantidad\t\tPrecio\n");
    for (int i = 0; i < factura.cantidadProductos; i++)
    {
        printf("%s\t\t%d\t\t%.2f\n", factura.productos[i].nombre,
               factura.productos[i].cantidad,
               factura.productos[i].precio);
    }

    printf("Total: %.2f\n", factura.total);
}

void readFacturas()
{
    FILE *file = fopen("factura.data", "rb");
    struct Factura factura;
    int opc =0;
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }
    else
    {
        printf("Cedula\t\tNombre\t\tTotal\n");
        while (fread(&factura, sizeof(struct Factura), 1, file))
        {
            imprimirFacturas(factura);
        }
    }

    fclose(file);

    printf("Desea ver el detalle de alguna factura? 1.Si/2.No: ");
    scanf("%d", &opc);
    if (opc == 1)
    {
        printf("Ingrese la cedula de la factura: ");
        int cedula;
        scanf("%d", &cedula);
        findFacturaByCedula(cedula);
    }
}

int findFacturaByCedula(int cedula)
{
    FILE *file = fopen("factura.data", "rb");
    struct Factura factura;
    int pos = 0, flag = 0;
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        return 0;
    }
    else
    {
        while (fread(&factura, sizeof(struct Factura), 1, file))
        {
            if (factura.cedula == cedula)
            {
                flag = 1;
                imprimirDeatalleFactura(factura);
                break;
            }
            pos = ftell(file);
        }
        if (flag == 0)
        {
            pos = -1;
            printf("No se encontro la factura\n");
        }
    }

    fclose(file);
    return pos;
}