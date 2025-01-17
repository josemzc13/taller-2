#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#define MAX_COMPONENTES 10
#define MAX_ORDENES 100
#define MAX_PRODUCTOS 5
 
// Productos electrónicos ofertados
char *productosFijos[] = {
    "Luz Intermitente",
    "Amplificador Basico",
    "Detector de Movimiento",
    "Cargador USB",
    "Termometro Digital"
};
 
// Componentes disponibles
char *componentes[] = {
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
 
// Ordenes de trabajo (guardadas en archivos)
int idOrden[MAX_ORDENES];
int idProducto[MAX_ORDENES];
int cantidad[MAX_ORDENES];
int tiempoTotal[MAX_ORDENES];
int totalOrdenes = 0;
 
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
void modificarStock() {
    int id, nuevoStock;
    printf("Ingrese el ID del componente a modificar (1-10): ");
    scanf("%d", &id);
    if (id < 1 || id > MAX_COMPONENTES) {
        printf("ID invalido.\n");
        return;
    }
 
    printf("Componente seleccionado: %s\n", componentes[id - 1]);
    printf("Ingrese la nueva cantidad en stock: ");
    scanf("%d", &nuevoStock);
   
    inventario[id - 1] = nuevoStock;
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
 
    printf("Producto seleccionado: %s\n", productosFijos[idProductoSeleccionado - 1]);
    printf("Ingrese la cantidad deseada: ");
    scanf("%d", &cantidadSeleccionada);
 
    // Verificar si se puede realizar la orden
    for (int i = 0; i < MAX_COMPONENTES; i++) {
        if (productosComponentes[idProductoSeleccionado - 1][i] * cantidadSeleccionada > inventario[i]) {
            printf("Orden no procede. Componente insuficiente: %s\n", componentes[i]);
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
    tiempoTotal[totalOrdenes] = cantidadSeleccionada * 120;  // Tiempo promedio de preparación
 
    totalOrdenes++;
    printf("Orden ingresada correctamente. Tiempo total de preparacion: %d minutos\n", tiempoTotal[totalOrdenes - 1]);
}
 
// Mostrar todas las órdenes
void mostrarOrdenes() {
    printf("\n--- Ordenes de Trabajo ---\n");
    for (int i = 0; i < totalOrdenes; i++) {
        printf("ID Orden: %d, Producto: %s, Cantidad: %d, Tiempo Total: %d minutos\n",
               idOrden[i],
               productosFijos[idProducto[i] - 1],
               cantidad[i],
               tiempoTotal[i]);
    }
}
 
// Buscar una orden por ID
void buscarOrden() {
    int id;
    printf("Ingrese el ID de la orden a buscar: ");
    scanf("%d", &id);
 
    for (int i = 0; i < totalOrdenes; i++) {
        if (idOrden[i] == id) {
            printf("Orden encontrada: Producto: %s, Cantidad: %d, Tiempo Total: %d minutos\n",
                   productosFijos[idProducto[i] - 1],
                   cantidad[i],
                   tiempoTotal[i]);
            return;
        }
    }
 
    printf("Orden no encontrada.\n");
}
 
// Menú principal
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
