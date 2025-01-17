struct Producto
{
    char nombre[50];
    int cantidad;
    float precio;
};

struct Factura
{
    char nombre[20];
    int cedula;
    int cantidadProductos;
    float total;
    struct Producto productos[5];
    int activo;
};

int menu();
void save(struct Factura *factura);
void readFacturas();
void createFactura();
int leerCadena(char *cadena, int tam);
int leerEntero(int *valor);

void imprimirFacturas(struct Factura factura);
void imprimirDeatalleFactura(struct Factura factura);
int findFacturaByCedula(int cedula);

void updateFactura();
void deleteFactura();
