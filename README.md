# UltiloTaller-EstructuraDeDatos Brayan Cortes Borquez 21.158.901-9
-- Comando g++ main.cpp -o main , ./main

Explicacion: 
El algoritmo de Bellman-Ford te sirve para encontrar el camino más corto entre un punto de partida y todos los demás puntos en el grafo:
- Primero, se inicializan las distancias a todos los nodos desde el nodo de origen como infinito y los padres como -1. Esto significa que no sabemos cuánto cuesta llegar a cada nodo desde el origen, ni qué nodo viene antes en el camino más corto.
- Luego, se establece la distancia al nodo de origen en 0.
- Se verifica si la distancia hasta el destino a través del nodo actual es menor que la distancia actual almacenada en distancias[conexion.idDestino]. Si es así, se actualiza la distancia y se guarda el nuevo nodo como el padre.
- Al final, el mapa distancias tendrá las distancias más cortas desde el nodo de origen hasta todos los demás nodos, y el mapa padres tendrá los predecesores en el camino más corto.
