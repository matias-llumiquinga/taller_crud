struct Producto
{
    char nombre[50];
    int cantidad;
    float precio;
};

struct Factura
{
    char nombre[20];
    float cedula;
    int cantidadProductos;
    float total;
    struct Producto productos[5];
};

int menu();
void save(struct Factura *factura);
void readFacturas();
void createFactura();
void leerCadena(char *cadena, int tam);
void imprimirFacturas(struct Factura factura);
void imprimirDeatalleFactura(struct Factura factura);
int findFacturaByCedula(int cedula);
