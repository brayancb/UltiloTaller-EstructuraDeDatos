#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <iomanip>
#include <limits>

using namespace std;

class Conexion {
public:
    int idOrigen;
    int idDestino;
    int velocidad;
    int distancia;
};

class Nodo {
public:
    int id;
    string nombre;
    vector<Conexion> conexiones;

    // Constructor por defecto
    Nodo() : id(-1), nombre(""), conexiones() {}

    // Constructor con parámetros
    Nodo(int id, const string& nombre) : id(id), nombre(nombre), conexiones() {}

    // Método para agregar una conexión
    void agregarConexion(int idOrigen, int idDestino, int velocidad, int distancia) {
        conexiones.push_back({idOrigen, idDestino, velocidad, distancia});
    }
};

unordered_map<int, Nodo> grafo;

void cargarGrafoDesdeCSV(const string& archivoServidores, const string& archivoConexiones) {
    ifstream archivoServidoresStream(archivoServidores);
    if (archivoServidoresStream.is_open()) {
        string linea;
        bool primeraLinea = true;
        while (getline(archivoServidoresStream, linea)) {
            if (primeraLinea) {
                primeraLinea = false;
                continue;
            }
            try {
                istringstream ss(linea);
                string token;
                getline(ss, token, ','); // ID
                int id = stoi(token);
                getline(ss, token, ','); // Nombre

                grafo[id] = Nodo(id, token);
            } catch (const invalid_argument& e) {
                cerr << "Error de conversión en la línea: " << linea << endl;
            }
        }
        archivoServidoresStream.close();
    } else {
        cerr << "Error al abrir el archivo de servidores." << endl;
    }

    ifstream archivoConexionesStream(archivoConexiones);
    if (archivoConexionesStream.is_open()) {
        string linea;
        bool primeraLinea = true;
        while (getline(archivoConexionesStream, linea)) {
            if (primeraLinea) {
                primeraLinea = false;
                continue;
            }
            try {
                istringstream ss(linea);
                string token;
                getline(ss, token, ','); // ID origen
                int idOrigen = stoi(token);
                getline(ss, token, ','); // ID destino
                int idDestino = stoi(token);
                getline(ss, token, ','); // Velocidad
                int velocidad = stoi(token);
                getline(ss, token, ','); // Distancia
                int distancia = stoi(token);

                grafo[idOrigen].agregarConexion(idOrigen, idDestino, velocidad, distancia);
            } catch (const invalid_argument& e) {
                cerr << "Error de conversión en la línea: " << linea << endl;
            }
        }
        archivoConexionesStream.close();
    } else {
        cerr << "Error al abrir el archivo de conexiones." << endl;
    }
}


// Implementación del algoritmo de Bellman-Ford
void bellmanFord(int origen, unordered_map<int, double>& distancias, unordered_map<int, int>& padres) {
    int n = grafo.size();

    // Inicializar distancias y padres
    for (const auto& par : grafo) {
        int idNodo = par.first;
        distancias[idNodo] = numeric_limits<double>::infinity();
        padres[idNodo] = -1;
    }

    distancias[origen] = 0;

    // Relajar aristas repetidamente
    for (int i = 0; i < n - 1; ++i) {
        for (const auto& par : grafo) {
            int idNodo = par.first;
            const Nodo& nodo = par.second;

            for (const Conexion& conexion : nodo.conexiones) {
                if (distancias[conexion.idDestino] > distancias[idNodo] + conexion.distancia) {
                    distancias[conexion.idDestino] = distancias[idNodo] + conexion.distancia;
                    padres[conexion.idDestino] = idNodo;
                }
            }
        }
    }
}

vector<int> obtenerCamino(int origen, int destino, const unordered_map<int, int>& padres) {
    vector<int> camino;
    int actual = destino;

    while (actual != origen) {
        if (actual == -1) {
            cerr << "Error: No se puede encontrar un camino al nodo " << actual << " desde el nodo de origen.\n";
            break;
        }

        camino.insert(camino.begin(), actual);
        actual = padres.at(actual);
    }

    return camino;
}

// Función para calcular la latencia y mostrar resultados
double calcularLatencia(int idOrigen, int idDestino, double tamanoArchivoMB) {
    // Aplicar Bellman-Ford para encontrar el mejor camino
    unordered_map<int, double> distancias;
    unordered_map<int, int> padres;
    bellmanFord(idOrigen, distancias, padres);

    // Obtener el camino óptimo
    vector<int> camino = obtenerCamino(idOrigen, idDestino, padres);

    // Mostrar el camino óptimo
    cout << "El mejor camino entre " << idOrigen << " y " << idDestino << " es: ";
    for (int nodo : camino) {
        cout << nodo << " -> ";
    }
    cout << "\n";

    // Calcular la latencia total
    double tiempoConexionPorParte = 1.0; // Tiempo de conexión por cada parte en segundos

    // Dividir el archivo en partes de 300 MB
    int numPartes = static_cast<int>(ceil(tamanoArchivoMB / 300.0));
    // Calcular el tiempo total teniendo en cuenta la división en partes
    double latenciaTotal = (camino.size() - 1) * tiempoConexionPorParte * numPartes;

    cout << fixed << setprecision(2);
    cout << "Tiempo total estimado para la transferencia: " << tamanoArchivoMB << " MB en "
         << latenciaTotal << " segundos.\n";

    return latenciaTotal;
}