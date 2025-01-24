//Mauricio Javier Muñoz Guerrero
#include <iostream>
#include <vector>
// Leer archivo txt
#include <fstream> 
#include <sstream>
// Poner el "infinito"
#include <climits> 
#include <string>
// Para usar reverse
#include <algorithm> 
using namespace std;

class Nodo {
public:
    char letra;
    int peso;
    vector<Nodo*> hijos;

    Nodo(char _letra, int _peso) {letra = _letra; peso = _peso;}
};

vector<vector<int>> leerGrafo(string &archivo, int &n) {
    ifstream txt(archivo);
    if (!txt) {cout << "El grafo no se puede leer." << endl; return {};}
    txt >> n; // cantidad o tamaño de la matriz
    vector<vector<int>> grafo(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        string linea;
        getline(txt >> ws, linea);
        stringstream ss(linea);
        string valor;

        for (int j = 0; j < n; j++) {
            getline(ss, valor, ',');
            grafo[i][j] = stoi(valor);
        }
    }
    txt.close();

    cout << "Leyendo el grafo..." << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << grafo[i][j] << (j < n - 1 ? " " : "");
        }
        cout << endl;
    }
    return grafo;
}

vector<int> dijkstra(vector<vector<int>> &grafo, int inicio, int destino, vector<int> &predecesores) {
    int n = grafo.size();
    vector<int> distancias(n, INT_MAX);
    vector<bool> visitados(n, false);

    distancias[inicio] = 0;
    for (int i = 0; i < n; i++) {
        int u = -1;
        for (int j = 0; j < n; j++) {
            if (!visitados[j] && (u == -1 || distancias[j] < distancias[u])) {
                u = j;
            }
        }
        if (distancias[u] == INT_MAX) break;
        visitados[u] = true;
        for (int k = 0; k < n; k++) {
            if (grafo[u][k] != 0 && distancias[u] + grafo[u][k] < distancias[k]) {
                distancias[k] = distancias[u] + grafo[u][k];
                predecesores[k] = u;
            }
        }
    }
    return distancias;
}

Nodo* construirArbol(int inicio, vector<int> &predecesores, vector<int> &distancias) {
    Nodo* raiz = new Nodo('A' + inicio, 0); 
    vector<Nodo*> nodos(predecesores.size(), nullptr);
    nodos[inicio] = raiz;

    for (int i = 0; i < predecesores.size(); i++) {
        //Casting para que inicio represente un tamano
        if (i != (size_t)inicio && predecesores[i] != -1) {
            Nodo* padre = nodos[predecesores[i]];
            if (padre == nullptr) {
                // Si el padre no existe, creamos uno nuevo
                padre = new Nodo('A' + predecesores[i], distancias[predecesores[i]]);
                nodos[predecesores[i]] = padre;
            }
            Nodo* nuevo = new Nodo('A' + i, distancias[i]);
            padre -> hijos.push_back(nuevo);
            nodos[i] = nuevo;
        }
    }
    return raiz;
}

void imprimirCamino(Nodo* raiz, int destino, vector<int> &predecesores, vector<int> &distancias) {
    if (!raiz) {cout << "Error: La raíz del árbol es nula." << endl; return;}
    vector<char> camino;
    int actual = destino; // Empezamos desde el destino

    // Reconstruir el camino desde el destino hasta la raíz
    while (actual != 0) {
        camino.push_back(char('A' + actual));
        actual = predecesores[actual];
        if (actual == -1) {
            cout << "No se pudo encontrar un camino desde A hasta " << char('A' + destino) << "." << endl;
            return;
        }
    }
    // Añadir la raíz 'A' al inicio del camino
    camino.push_back('A');
    // Invertir el camino para que vaya de 'A' al destino
    reverse(camino.begin(), camino.end());
    // Imprimir el camino
    cout << "El camino mas corto: " << raiz->letra << " hasta " << char('A' + destino) << ": ";
    for (int i = 0; i < camino.size(); i++) {
        cout << camino[i] << (i < camino.size() - 1 ? " -> " : "");
    }
    cout << endl;
    cout << "La distancia mas corta: " << distancias[destino] << endl;
}

int main() {
    string archivo = "grafo.txt"; // Archivo predefinido
    int n;
    vector<vector<int>> grafo = leerGrafo(archivo, n);
    if (grafo.empty()) return 1;
    if (n <= 0 || n > 26) {
        cout << "Matriz invalida, no coincide con la cantidad del alfabeto -> " << n << endl;
        exit(1); // Salir del programa con un código de error
    }
    char destinoChar;
    while (true) {
        cout << "Ingrese el nodo destino (de letra A hasta "<<char('A' + n - 1)<<"): ";
        cin >> destinoChar;
        if (destinoChar < 'A' || destinoChar >= 'A' + n) {
            cout << "Error: Nodo destino fuera de rango. Por favor, ingrese un nodo entre A y " << char('A' + n - 1) << "." << endl;
            continue;
        }
        int destino = destinoChar - 'A';
        vector<int> predecesores(n, -1);
        vector<int> distancias = dijkstra(grafo, 0, destino, predecesores);
        Nodo* arbol = construirArbol(0, predecesores, distancias);
        imprimirCamino(arbol, destino, predecesores, distancias);
        break;
    }
    return 0;
}