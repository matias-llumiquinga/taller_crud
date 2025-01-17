#include <stdio.h>
#include <string.h>
#include "funciones.h"

int menu()
{
    int opc;
    printf("1. Crear factura\n");
    printf("2. Ver facturas\n");
    printf("3. Actualizar factura\n");
    printf("4. Eliminar factura\n");
    printf("5. Salir\n");
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

int leerCadena(char *cadena, int tam)
{
    int continuar = 0;
    strcpy(cadena, "");
    while (cadena[0] == '\0' || cadena[0] == '\n')
    {
        fgets(cadena, tam, stdin);
    }
    int len = strlen(cadena) - 1;
    if (cadena[len] != '\n')
    {
        printf("-HA EXCEDIDO EL NUMERO MAXIMO DE CARACTERES-\n");
        while (getchar() != '\n')
            ;
    }
    else
    {
        cadena[len] = '\0';
        continuar = 1;
    }
    return continuar;
}

int leerEntero(int *valor)
{
    int numI = 0, continuar = 0;
    float numF = 0;
    scanf("%f", &numF);
    numI = (int)numF;
    if (numF == numI)
    {
        *valor = numI;
        continuar = 1;
    }
    else
    {
        printf("-NO INGRESE NUMEROS DECIMALES-\n");
    }

    return continuar;
}

void createFactura()
{
    struct Factura factura;
    int continuar = 0;
    do
    {
        printf("Ingrese el nombre del cliente: ");
        continuar = leerCadena(factura.nombre, 20);
    } while (continuar == 0);
    do
    {
        printf("Ingrese la cedula del cliente: ");
        continuar = leerEntero(&factura.cedula);
        if (factura.cedula <= 0 && continuar == 1)
        {
            continuar = 0;
            printf("-VALOR FUERA DE RANGO-\n");
        }
    } while (continuar == 0);

    do
    {
        printf("Ingrese la cantidad de productos (1-5): ");
        continuar = leerEntero(&factura.cantidadProductos);
        if (factura.cantidadProductos <= 0 && continuar == 1 || factura.cantidadProductos > 5 && continuar == 1)
        {
            continuar = 0;
            printf("-VALOR FUERA DE RANGO-\n");
        }
    } while (continuar == 0);

    factura.total = 0;
    for (int i = 0; i < factura.cantidadProductos; i++)
    {
        do
        {
            printf("Ingrese el nombre del producto %d: ", i + 1);
            continuar = leerCadena(factura.productos[i].nombre, 50);
        } while (continuar == 0);

        do
        {
            printf("Ingrese la cantidad del producto (1-10): ");
            continuar = leerEntero(&factura.productos[i].cantidad);
            if (factura.productos[i].cantidad <= 0 && continuar == 1 || factura.productos[i].cantidad > 10 && continuar == 1)
            {
                continuar = 0;
                printf("-VALOR FUERA DE RANGO-\n");
            }
        } while (continuar == 0);

        do
        {
            printf("Ingrese el precio del producto %d (0<precio<=1000): ", i + 1);
            scanf("%f", &factura.productos[i].precio);
            if (factura.productos[i].precio <= 0 || factura.productos[i].precio > 1000)
            {
                continuar = 0;
                printf("-VALOR FUERA DE RANGO-\n");
            }
            else
            {
                continuar = 1;
            }
        } while (continuar == 0);

        factura.total += factura.productos[i].cantidad * factura.productos[i].precio;
    }
    factura.activo = 1;
    save(&factura);
}

void imprimirFacturas(struct Factura factura)
{
    if (factura.activo == 1)
    {
        printf("%-15d%-25s%-15.2f\n", factura.cedula, factura.nombre, factura.total);
    }
}

void imprimirDeatalleFactura(struct Factura factura)
{
    printf("-DETALLE DE LA FACTURA-\n");
    printf("Cedula: %d\n", factura.cedula);
    printf("Nombre: %s\n", factura.nombre);

    printf("%-55s%-15s%-15s\n", "Nombre del Producto", "Cantidad", "Precio");

    for (int i = 0; i < factura.cantidadProductos; i++)
    {
        printf("%-55s%-15d%-15.2f\n", factura.productos[i].nombre,
               factura.productos[i].cantidad,
               factura.productos[i].precio);
    }

    printf("Total: %.2f\n", factura.total);
}

void readFacturas()
{
    FILE *file = fopen("factura.data", "rb");
    struct Factura factura;
    int opc = 0, pos = 0;
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }
    else
    {
        printf("%-15s%-25s%-15s\n", "Cedula", "Nombre", "Total");
        while (fread(&factura, sizeof(struct Factura), 1, file))
        {
            imprimirFacturas(factura);
        }
    }

    printf("Desea ver el detalle de alguna factura? 1.Si/2.No: ");
    scanf("%d", &opc);
    if (opc == 1)
    {
        printf("Ingrese la cedula de la factura: ");
        int cedula;
        scanf("%d", &cedula);
        pos = findFacturaByCedula(cedula);

        if (pos >= 0)
        {
            fseek(file, pos, SEEK_SET);
            fread(&factura, sizeof(struct Factura), 1, file);
            imprimirDeatalleFactura(factura);
        }
    }

    fclose(file);
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
                break;
            }
            pos = ftell(file);
        }
        if (flag == 0 || factura.activo == 0)
        {
            pos = -1;
            printf("No se encontro la factura\n");
        }
    }

    fclose(file);
    return pos;
}

void updateFactura()
{
    FILE *file = fopen("factura.data", "r+b");
    struct Factura factura;
    int opc = 0, pos = 0;
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }
    else
    {
        printf("Ingrese la cedula de la factura a actualizar: ");
        int cedula;
        scanf("%d", &cedula);
        pos = findFacturaByCedula(cedula);
        if (pos >= 0)
        {
            fseek(file, pos, SEEK_SET);
            int continuar = 0;
            do
            {
                printf("Ingrese el nombre del cliente: ");
                continuar = leerCadena(factura.nombre, 20);
            } while (continuar == 0);
            do
            {
                printf("Ingrese la cedula del cliente: ");
                continuar = leerEntero(&factura.cedula);
                if (factura.cedula <= 0 && continuar == 1)
                {
                    continuar = 0;
                    printf("-VALOR FUERA DE RANGO-\n");
                }
            } while (continuar == 0);

            do
            {
                printf("Ingrese la cantidad de productos (1-5): ");
                continuar = leerEntero(&factura.cantidadProductos);
                if (factura.cantidadProductos <= 0 && continuar == 1 || factura.cantidadProductos > 5 && continuar == 1)
                {
                    continuar = 0;
                    printf("-VALOR FUERA DE RANGO-\n");
                }
            } while (continuar == 0);

            factura.total = 0;
            for (int i = 0; i < factura.cantidadProductos; i++)
            {
                do
                {
                    printf("Ingrese el nombre del producto %d: ", i + 1);
                    continuar = leerCadena(factura.productos[i].nombre, 50);
                } while (continuar == 0);

                do
                {
                    printf("Ingrese la cantidad del producto (1-10): ");
                    continuar = leerEntero(&factura.productos[i].cantidad);
                    if (factura.productos[i].cantidad <= 0 && continuar == 1 || factura.productos[i].cantidad > 10 && continuar == 1)
                    {
                        continuar = 0;
                        printf("-VALOR FUERA DE RANGO-\n");
                    }
                } while (continuar == 0);

                do
                {
                    printf("Ingrese el precio del producto %d (0<precio<=1000): ", i + 1);
                    scanf("%f", &factura.productos[i].precio);
                    if (factura.productos[i].precio <= 0 || factura.productos[i].precio > 1000)
                    {
                        continuar = 0;
                        printf("-VALOR FUERA DE RANGO-\n");
                    }
                    else
                    {
                        continuar = 1;
                    }
                } while (continuar == 0);

                factura.total += factura.productos[i].cantidad * factura.productos[i].precio;
            }
            factura.activo = 1;
            fwrite(&factura, sizeof(struct Factura), 1, file);
        }
    }

    fclose(file);
}

void deleteFactura()
{
    FILE *file = fopen("factura.data", "rb+");
    struct Factura factura;
    struct Factura facturaNula = {0};
    int opc = 0, pos = 0;
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }
    else
    {
        printf("Ingrese la cedula de la factura a actualizar: ");
        int cedula;
        scanf("%d", &cedula);
        pos = findFacturaByCedula(cedula);
        if (pos >= 0)
        {
            fseek(file, pos, SEEK_SET);
            fwrite(&facturaNula, sizeof(struct Factura), 1, file);
            printf("-FACTURA ELIMINADA EXITOSAMENTE-\n");
        }
    }

    fclose(file);
}
