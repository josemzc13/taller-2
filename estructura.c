#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMPONENTES 10
#define MAX_ORDENES 100
#define MAX_PRODUCTOS 5

// Estructuras para organizar datos
typedef struct {
    char nombre[50];
    int stock;
} Componente;

typedef struct {
    char nombre[50];
    int componentesNecesarios[MAX_COMPONENTES];
} Producto;

typedef struct {
    int id;
    int idProducto;
    int cantidad;
    int tiempoTotal;
} Orden;

// Variables globales
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

// Funciones para guardar y cargar datos desde archivos
void guardarInventario() {
    FILE *archivo = fopen("inventario.txt", "w");
    if (!archivo) {
        printf("No se pudo abrir el archivo de inventario.\n");
        return;
    }

    for (int i = 0; i < MAX_COMPONENTES; i++) {
        fprintf(archivo, "%d\n", componentes[i].stock);
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
        fscanf(archivo, "%d", &componentes[i].stock);
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
        fprintf(archivo, "%d %d %d %d\n", ordenes[i].id, ordenes[i].idProducto, ordenes[i].cantidad, ordenes[i].tiempoTotal);
    }

    fclose(archivo);
}

void cargarOrdenes() {
    FILE *archivo = fopen("ordenes.txt", "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo de ordenes.\n");
        return;
    }

    while (fscanf(archivo, "%d %d %d %d", &ordenes[totalOrdenes].id, &ordenes[totalOrdenes].idProducto, &ordenes[totalOrdenes].cantidad, &ordenes[totalOrdenes].tiempoTotal) != EOF) {
        totalOrdenes++;
    }

    fclose(archivo);
}

// Mostrar inventario
void mostrarInventario() {
    printf("\n--- Inventario ---\n");
    for (int i = 0; i < MAX_COMPONENTES; i++) {
        printf("Componente: %s, Stock: %d\n", componentes[i].nombre, componentes[i].stock);
    }
}

// Mostrar productos ofertados
void mostrarProductosOfertados() {
    printf("\n--- Productos Ofertados ---\n");
    for (int i = 0; i < MAX_PRODUCTOS; i++) {
        printf("Producto: %s\n", productos[i].nombre);
        printf("Componentes necesarios:\n");
        for (int j = 0; j < MAX_COMPONENTES; j++) {
            if (productos[i].componentesNecesarios[j] > 0) {
                printf("- %s: %d\n", componentes[j].nombre, productos[i].componentesNecesarios[j]);
            }
        }
        printf("\n");
    }
}

// Modificar stock de componentes
void modificarStock() {
    int id, nuevoStock;
    printf("Ingrese el ID del componente a modificar (1-10): ");
    scanf("%d", &id);
    if (id < 1 || id > MAX_COMPONENTES) {
        printf("ID invalido.\n");
        return;
    }

    printf("Componente seleccionado: %s\n", componentes[id - 1].nombre);
    printf("Ingrese la nueva cantidad en stock: ");
    scanf("%d", &nuevoStock);
   
    componentes[id - 1].stock = nuevoStock;
    printf("Stock actualizado correctamente.\n");
}

// Ingresar una nueva orden
void ingresarOrden() {
    int idProductoSeleccionado, cantidadSeleccionada;
    printf("Ingrese el ID del producto a pedir (1-5): ");
    scanf("%d", &idProductoSeleccionado);

    if (idProductoSeleccionado < 1 || idProductoSeleccionado > MAX_PRODUCTOS) {
        printf("ID de producto invalido.\n");
        return;
    }

    printf("Producto seleccionado: %s\n", productos[idProductoSeleccionado - 1].nombre);
    printf("Ingrese la cantidad deseada: ");
    scanf("%d", &cantidadSeleccionada);

    // Verificar si se puede realizar la orden
    for (int i = 0; i < MAX_COMPONENTES; i++) {
        if (productos[idProductoSeleccionado - 1].componentesNecesarios[i] * cantidadSeleccionada > componentes[i].stock) {
            printf("Orden no procede. Componente insuficiente: %s\n", componentes[i].nombre);
            return;
        }
    }

    // Actualizar inventario y agregar orden
    for (int i = 0; i < MAX_COMPONENTES; i++) {
        componentes[i].stock -= productos[idProductoSeleccionado - 1].componentesNecesarios[i] * cantidadSeleccionada;
    }

    ordenes[totalOrdenes].id = totalOrdenes + 1;
    ordenes[totalOrdenes].idProducto = idProductoSeleccionado;
    ordenes[totalOrdenes].cantidad = cantidadSeleccionada;
    ordenes[totalOrdenes].tiempoTotal = cantidadSeleccionada * 120;  // Tiempo promedio de preparacion

    totalOrdenes++;
    printf("Orden ingresada correctamente. Tiempo total de preparacion: %d minutos\n", ordenes[totalOrdenes - 1].tiempoTotal);
}

// Mostrar todas las ordenes
void mostrarOrdenes() {
    printf("\n--- Ordenes de Trabajo ---\n");
    for (int i = 0; i < totalOrdenes; i++) {
        printf("ID Orden: %d, Producto: %s, Cantidad: %d, Tiempo Total: %d minutos\n",
               ordenes[i].id,
               productos[ordenes[i].idProducto - 1].nombre,
               ordenes[i].cantidad,
               ordenes[i].tiempoTotal);
    }
}

// Buscar una orden por ID
void buscarOrden() {
    int id;
    printf("Ingrese el ID de la orden a buscar: ");
    scanf("%d", &id);

    for (int i = 0; i < totalOrdenes; i++) {
        if (ordenes[i].id == id) {
            printf("Orden encontrada: Producto: %s, Cantidad: %d, Tiempo Total: %d minutos\n",
                   productos[ordenes[i].idProducto - 1].nombre,
                   ordenes[i].cantidad,
                   ordenes[i].tiempoTotal);
            return;
        }
    }

    printf("Orden no encontrada.\n");
}

// Menu principal
void menu() {
    int opcion;
    do {
        printf("\n--- Menu ---\n");
        printf("1. Mostrar productos ofertados\n");
        printf("2. Mostrar inventario\n");
        printf("3. Modificar stock\n");
        printf("4. Ingresar orden de trabajo\n");
        printf("5. Mostrar ordenes de trabajo\n");
        printf("6. Buscar orden\n");
        printf("7. Guardar y salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: mostrarProductosOfertados(); break;
            case 2: mostrarInventario(); break;
            case 3: modificarStock(); break;
            case 4: ingresarOrden(); break;
            case 5: mostrarOrdenes(); break;
            case 6: buscarOrden(); break;
            case 7:
                guardarInventario();
                guardarOrdenes();
                printf("Datos guardados. Saliendo...\n");
                break;
            default: printf("Opcion invalida.\n");
        }
    } while (opcion != 7);
}

// Funcion principal
int main() {
    cargarInventario();
    cargarOrdenes();
    menu();
    return 0;
}
