#include <iostream>
#include "grafo.h"
using namespace std;

class menu{ 
    public:
    void ejecutarMenu() {
        // Cargar el grafo desde archivos CSV
        cargarGrafoDesdeCSV("servidores.csv", "conexiones.csv");
            // Enviar el archivo y mostrar información
        int idOrigen, idDestino;
        double tamanoArchivoMB;

        cout << "Ingrese la ID del cliente o router que envía el archivo: ";
        cin >> idOrigen;

        cout << "Ingrese la ID del cliente o router que recibe el archivo: ";
        cin >> idDestino;

        cout << "Ingrese el tamaño del archivo en MB: ";
        cin >> tamanoArchivoMB;

        // Calcular la latencia y mostrar resultados
        calcularLatencia(idOrigen, idDestino, tamanoArchivoMB);
    }
};