#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMPONENTES 15
#define MAX_ORDENES 100
#define MAX_PRODUCTOS 5
#define MAX_NOMBRE 50

typedef struct {
    char nombre[MAX_NOMBRE];
    int cantidad;
} Componente;

typedef struct {
    char nombre[MAX_NOMBRE];
    int componentesNecesarios[MAX_COMPONENTES];
} Producto;

typedef struct {
    int id;
    int idProducto;
    int cantidad;
    int tiempoPreparacion;
} Orden;

Componente componentes[MAX_COMPONENTES] = {
    {"Resistencias", 1000},
    {"Condensadores", 500},
    {"LEDs", 200},
    {"Transistores", 300},
    {"Microcontroladores", 50},
    {"Cables", 1000},
    {"Sensores de Temperatura", 100},
    {"Sensores de Movimiento", 50},
    {"Conectores", 200},
    {"Placas de Circuito", 100}
};

Producto productos[MAX_PRODUCTOS] = {
    {"Luz Intermitente", {10, 0, 2, 1, 0, 5, 0, 0, 1, 1}},
    {"Amplificador Basico", {5, 2, 0, 1, 0, 10, 0, 0, 1, 1}},
    {"Detector de Movimiento", {5, 3, 1, 1, 0, 5, 0, 1, 1, 1}},
    {"Cargador USB", {5, 5, 0, 2, 1, 5, 0, 0, 1, 1}},
    {"Termometro Digital", {10, 5, 0, 0, 1, 5, 1, 0, 1, 1}}
};

Orden ordenes[MAX_ORDENES];
int totalOrdenes = 0;
int totalComponentes = 10;

void agregarComponente() {
    if (totalComponentes >= MAX_COMPONENTES) {
        printf("No se pueden agregar mas componentes. Capacidad maxima alcanzada.\n");
        return;
    }

    Componente nuevoComponente;
    printf("Ingrese el nombre del nuevo componente: ");
    getchar();
    fgets(nuevoComponente.nombre, MAX_NOMBRE, stdin);
    nuevoComponente.nombre[strcspn(nuevoComponente.nombre, "\n")] = '\0';

    printf("Ingrese la cantidad inicial en inventario: ");
    scanf("%d", &nuevoComponente.cantidad);

    if (nuevoComponente.cantidad < 0) {
        printf("La cantidad inicial no puede ser negativa.\n");
        return;
    }

    componentes[totalComponentes] = nuevoComponente;
    for (int i = 0; i < MAX_PRODUCTOS; i++) {
        productos[i].componentesNecesarios[totalComponentes] = 0;
    }

    totalComponentes++;
    printf("Componente agregado correctamente. Nuevo total de componentes: %d\n", totalComponentes);
}

void guardarInventario() {
    FILE *archivo = fopen("inventario.txt", "w");
    if (!archivo) {
        printf("No se pudo abrir el archivo de inventario.\n");
        return;
    }

    for (int i = 0; i < totalComponentes; i++) {
        fprintf(archivo, "%s %d\n", componentes[i].nombre, componentes[i].cantidad);
    }

    fclose(archivo);
}

void cargarInventario() {
    FILE *archivo = fopen("inventario.txt", "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo de inventario.\n");
        return;
    }

    totalComponentes = 0;
    while (fscanf(archivo, "%s %d", componentes[totalComponentes].nombre, &componentes[totalComponentes].cantidad) != EOF) {
        totalComponentes++;
    }

    fclose(archivo);
}

void mostrarInventario() {
    printf("\n--- Inventario ---\n");
    for (int i = 0; i < totalComponentes; i++) {
        printf("Componente: %s, Stock: %d\n", componentes[i].nombre, componentes[i].cantidad);
    }
}

void mostrarProductosOfertados() {
    printf("\n--- Productos Ofertados ---\n");
    for (int i = 0; i < MAX_PRODUCTOS; i++) {
        printf("Producto: %s\n", productos[i].nombre);
        printf("Componentes necesarios:\n");
        for (int j = 0; j < totalComponentes; j++) {
            if (productos[i].componentesNecesarios[j] > 0) {
                printf("- %s: %d\n", componentes[j].nombre, productos[i].componentesNecesarios[j]);
            }
        }
        printf("\n");
    }
}

void ingresarOrden() {
    if (totalOrdenes >= MAX_ORDENES) {
        printf("No se pueden agregar mas ordenes. Capacidad maxima alcanzada.\n");
        return;
    }

    int idProductoSeleccionado, cantidadSeleccionada;
    printf("Ingrese el Codigo del producto a pedir (1-%d): ", MAX_PRODUCTOS);
    scanf("%d", &idProductoSeleccionado);

    if (idProductoSeleccionado < 1 || idProductoSeleccionado > MAX_PRODUCTOS) {
        printf("Codigo de producto invalido.\n");
        return;
    }

    printf("Producto seleccionado: %s\n", productos[idProductoSeleccionado - 1].nombre);
    printf("Ingrese la cantidad deseada: ");
    scanf("%d", &cantidadSeleccionada);

    for (int i = 0; i < totalComponentes; i++) {
        if (productos[idProductoSeleccionado - 1].componentesNecesarios[i] * cantidadSeleccionada > componentes[i].cantidad) {
            printf("Orden no procede. Componente insuficiente: %s\n", componentes[i].nombre);
            return;
        }
    }

    for (int i = 0; i < totalComponentes; i++) {
        componentes[i].cantidad -= productos[idProductoSeleccionado - 1].componentesNecesarios[i] * cantidadSeleccionada;
    }

    ordenes[totalOrdenes] = (Orden){
        .id = totalOrdenes + 1,
        .idProducto = idProductoSeleccionado,
        .cantidad = cantidadSeleccionada,
        .tiempoPreparacion = cantidadSeleccionada * 120
    };

    totalOrdenes++;
    printf("Orden ingresada correctamente. Tiempo total de preparacion: %d minutos\n", ordenes[totalOrdenes - 1].tiempoPreparacion);
}

void mostrarOrdenes() {
    printf("\n--- Ordenes de Trabajo ---\n");
    for (int i = 0; i < totalOrdenes; i++) {
        printf("Orden %d:\n", ordenes[i].id);
        printf("Producto: %s\n", productos[ordenes[i].idProducto - 1].nombre);
        printf("Cantidad: %d\n", ordenes[i].cantidad);
        printf("Tiempo total de preparacion: %d minutos\n", ordenes[i].tiempoPreparacion);
        printf("\n");
    }
}

int main() {
    int opcion;
    cargarInventario();

    do {
        printf("\n--- Menu Principal ---\n");
        printf("1. Mostrar Inventario\n");
        printf("2. Mostrar Productos Ofertados\n");
        printf("3. Ingresar Nueva Orden\n");
        printf("4. Mostrar Ordenes de Trabajo\n");
        printf("5. Agregar Componente\n");
        printf("6. Guardar y Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                mostrarInventario();
                break;
            case 2:
                mostrarProductosOfertados();
                break;
            case 3:
                ingresarOrden();
                break;
            case 4:
                mostrarOrdenes();
                break;
            case 5:
                agregarComponente();
                break;
            case 6:
                guardarInventario();
                printf("Hasta luego.\n");
                break;
            default:
                printf("Opcion invalida. Intente de nuevo.\n");
        }
    } while (opcion != 6);

    return 0;
}
