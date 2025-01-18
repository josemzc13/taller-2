#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMPONENTES 10
#define MAX_ORDENES 100
#define MAX_PRODUCTOS 5
#define MAX_NOMBRE 50

// Productos electronicos ofertados
char *productosFijos[] = {
    "Luz Intermitente",
    "Amplificador Basico",
    "Detector de Movimiento",
    "Cargador USB",
    "Termometro Digital"
};

// Componentes disponibles
char *componentes[MAX_COMPONENTES] = {
    "Resistencias",
    "Condensadores",
    "LEDs",
    "Transistores",
    "Microcontroladores",
    "Cables",
    "Sensores de Temperatura",
    "Sensores de Movimiento",
    "Conectores",
    "Placas de Circuito"
};

// Cantidades disponibles de componentes
int inventario[MAX_COMPONENTES] = {1000, 500, 200, 300, 50, 1000, 100, 50, 200, 100};

// Cantidades de componentes necesarios para cada producto
int productosComponentes[MAX_PRODUCTOS][MAX_COMPONENTES] = {
    {10, 0, 2, 1, 0, 5, 0, 0, 1, 1},    // Luz Intermitente
    {5, 2, 0, 1, 0, 10, 0, 0, 1, 1},    // Amplificador Basico
    {5, 3, 1, 1, 0, 5, 0, 1, 1, 1},     // Detector de Movimiento
    {5, 5, 0, 2, 1, 5, 0, 0, 1, 1},     // Cargador USB
    {10, 5, 0, 0, 1, 5, 1, 0, 1, 1}     // Termometro Digital
};

// Variables para el manejo de ordenes de trabajo
int idOrden[MAX_ORDENES];
int idProducto[MAX_ORDENES];
int cantidad[MAX_ORDENES];
int tiempoTotal[MAX_ORDENES];
int totalOrdenes = 0;
int totalComponentes = MAX_COMPONENTES; // Contador dinamico de componentes

// Funcion para agregar un nuevo componente
void agregarComponente() {
    if (totalComponentes > MAX_COMPONENTES) {
        printf("No se pueden agregar mas componentes. Capacidad maxima alcanzada.\n");
        return;
    }

    char nuevoComponente[MAX_NOMBRE];
    int cantidadInicial;

    printf("Ingrese el nombre del nuevo componente: ");
    getchar(); 
    fgets(nuevoComponente, MAX_NOMBRE, stdin);

    // Eliminar el salto de linea al final del nombre si existe
    size_t len = strlen(nuevoComponente);
    if (len > 0 && nuevoComponente[len - 1] == '\n') {
        nuevoComponente[len - 1] = '\0';
    }

    printf("Ingrese la cantidad inicial en inventario: ");
    scanf("%d", &cantidadInicial);

    // Validar cantidad inicial
    if (cantidadInicial < 0) {
        printf("La cantidad inicial no puede ser negativa.\n");
        return;
    }

    // Agregar el nuevo componente al sistema
    componentes[totalComponentes] = (char *)malloc(strlen(nuevoComponente) + 1);
    if (!componentes[totalComponentes]) {
        printf("Error al asignar memoria para el componente.\n");
        return;
    }

    strcpy(componentes[totalComponentes], nuevoComponente);
    inventario[totalComponentes] = cantidadInicial;

    // Inicializar las necesidades de productos a 0 para el nuevo componente
    for (int i = 0; i < MAX_PRODUCTOS; i++) {
        productosComponentes[i][totalComponentes] = 0;
    }

    totalComponentes++;
    printf("Componente agregado correctamente.\n Nuevo total de componentes: %d\n", totalComponentes);
}



// Funciones para guardar y cargar datos desde archivos
void guardarInventario() {
    FILE *archivo = fopen("inventario.txt", "w");
    if (!archivo) {
        printf("No se pudo abrir el archivo de inventario.\n");
        return;
    }

    for (int i = 0; i < MAX_COMPONENTES; i++) {
        fprintf(archivo, "%d\n", inventario[i]);
    }

    fclose(archivo);
}

void cargarInventario() {
    FILE *archivo = fopen("inventario.txt", "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo de inventario.\n");
        return;
    }

    for (int i = 0; i < MAX_COMPONENTES; i++) {
        fscanf(archivo, "%d", &inventario[i]);
    }

    fclose(archivo);
}

void guardarOrdenes() {
    FILE *archivo = fopen("ordenes.txt", "w");
    if (!archivo) {
        printf("No se pudo abrir el archivo de ordenes.\n");
        return;
    }

    for (int i = 0; i < totalOrdenes; i++) {
        fprintf(archivo, "%d %d %d %d\n", idOrden[i], idProducto[i], cantidad[i], tiempoTotal[i]);
    }

    fclose(archivo);
}

void cargarOrdenes() {
    FILE *archivo = fopen("ordenes.txt", "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo de ordenes.\n");
        return;
    }

    while (fscanf(archivo, "%d %d %d %d", &idOrden[totalOrdenes], &idProducto[totalOrdenes], &cantidad[totalOrdenes], &tiempoTotal[totalOrdenes]) != EOF) {
        totalOrdenes++;
    }

    fclose(archivo);
}

// Mostrar inventario
void mostrarInventario() {
    printf("\n--- Inventario ---\n");
    for (int i = 0; i < MAX_COMPONENTES; i++) {
        printf("Componente: %s, Stock: %d\n", componentes[i], inventario[i]);
    }
}

// Mostrar productos ofertados
void mostrarProductosOfertados() {
    printf("\n--- Productos Ofertados ---\n");
    for (int i = 0; i < MAX_PRODUCTOS; i++) {
        printf("Producto: %s\n", productosFijos[i]);
        printf("Componentes necesarios:\n");
        for (int j = 0; j < MAX_COMPONENTES; j++) {
            if (productosComponentes[i][j] > 0) {
                printf("- %s: %d\n", componentes[j], productosComponentes[i][j]);
            }
        }
        printf("\n");
    }
}

// Modificar stock de componentes
void Comprar() {
    int id, cantidadAgregar;
    printf("Ingrese el Codigo del componente a modificar (1-%d): ", totalComponentes);
    scanf("%d", &id);
    if (id < 1 || id > totalComponentes) {
        printf("Codigo invalido.\n");
        return;
    }

    printf("Componente seleccionado: %s\n", componentes[id - 1]);
    printf("Ingrese la cantidad comprada: ");
    scanf("%d", &cantidadAgregar);

    if (cantidadAgregar < 0) {
        printf("No se puede agregar una cantidad negativa.\n");
        return;
    }

    inventario[id - 1] += cantidadAgregar;
    printf("Stock actualizado correctamente.\n Nuevo stock de %s: %d\n", componentes[id - 1], inventario[id - 1]);
}


// Ingresar una nueva orden
void ingresarOrden() {
    int idProductoSeleccionado, cantidadSeleccionada;
    printf("Ingrese el Codigo del producto a pedir (1-5): ");
    scanf("%d", &idProductoSeleccionado);

    if (idProductoSeleccionado < 1 || idProductoSeleccionado > MAX_PRODUCTOS) {
        printf("Codigo de producto invalido.\n");
        return;
    }

    printf("Producto seleccionado: %s\n", productosFijos[idProductoSeleccionado - 1]);
    printf("Ingrese la cantidad deseada: ");
    scanf("%d", &cantidadSeleccionada);

    // Verificar si se puede realizar la orden
    for (int i = 0; i < MAX_COMPONENTES; i++) {
        if (productosComponentes[idProductoSeleccionado - 1][i] * cantidadSeleccionada > inventario[i]) {
            printf("Orden no procede. \n Componente insuficiente: %s\n", componentes[i]);
            return;
        }
    }

    // Actualizar inventario y agregar orden
    for (int i = 0; i < MAX_COMPONENTES; i++) {
        inventario[i] -= productosComponentes[idProductoSeleccionado - 1][i] * cantidadSeleccionada;
    }

    idOrden[totalOrdenes] = totalOrdenes + 1;
    idProducto[totalOrdenes] = idProductoSeleccionado;
    cantidad[totalOrdenes] = cantidadSeleccionada;
    tiempoTotal[totalOrdenes] = cantidadSeleccionada * 120;  // Tiempo promedio de preparacion

    totalOrdenes++;
    printf("Orden ingresada correctamente. \n Tiempo total de preparacion: %d minutos\n", tiempoTotal[totalOrdenes - 1]);
}

// Mostrar todas las ordenes
void mostrarOrdenes() {
    printf("\n--- Ordenes de Trabajo ---\n");
    for (int i = 0; i < totalOrdenes; i++) {
        printf("Orden %d:\n", idOrden[i]);
        printf("Producto: %s\n", productosFijos[idProducto[i] - 1]);
        printf("Cantidad: %d\n", cantidad[i]);
        printf("Tiempo total de preparacion: %d minutos\n", tiempoTotal[i]);
        printf("\n");
    }
}

// Menu principal
int main() {
    int opcion;

    cargarInventario();
    cargarOrdenes();

    do {
        printf("\n--- Menu Principal ---\n");
        printf("1. Mostrar Inventario\n");
        printf("2. Comprar Componentes\n");
        printf("3. Mostrar Productos Ofertados\n");
        printf("4. Ingresar Nueva Orden\n");
        printf("5. Mostrar Ordenes de Trabajo\n");
        printf("6. Agregar Componente\n");
        printf("7. Guardar y Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                mostrarInventario();
                break;
            case 2:
                Comprar();
                break;
            case 3:
                mostrarProductosOfertados();
                break;
            case 4:
                ingresarOrden();
                break;
            case 5:
                mostrarOrdenes();
                break;
            case 6:
                agregarComponente();
                break;
            case 7:
                guardarInventario();
                guardarOrdenes();
                printf("Hasta luego.\n");
                break;
            default:
                printf("Opcion invalida. Intente de nuevo.\n");
        }
    } while (opcion != 7);

    return 0;
}
