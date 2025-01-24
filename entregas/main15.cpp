// /JOAQUIN VEGA HERNANDEZ
#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <limits.h>
#include <stack>

using namespace std;

class Nodo {
    public:
        pair<char, int> datos;
        vector<Nodo*> hijos;

        Nodo(char num, int distancia) {
            datos = make_pair(num, distancia);
        }
};

void dijkstra(vector<vector<int>>& matriz, vector<int>& distancias, vector<Nodo*>& nodos) {
    int tam = matriz.size();
    distancias[0] = 0;

    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    pq.push({0,0});

    while (!pq.empty()) {
        int nodoActual = pq.top().second;
        int distActual = pq.top().first;
        pq.pop();

        if (distActual > distancias[nodoActual]) {
            continue;
        }

        for (int i=0; i<tam; i++) {
            if (matriz[nodoActual][i] != 0) {
                int dist = distActual + matriz[nodoActual][i];
                if (dist < distancias[i]) {
                    distancias[i] = dist;

                    Nodo* nodo = new Nodo(i+'A', dist);
                    nodos[i] = nodo;
                    nodos[nodoActual]->hijos.push_back(nodo);
                    pq.push({dist, i});
                }
            }
        }
    }
}

void dfs(Nodo* nodo, vector<bool>& visitados, vector<char>& camino, char nodoIr) {
    visitados[nodo->datos.first - 'A'] = true;
    camino.push_back(nodo->datos.first);

    if (nodo->datos.first == nodoIr) {
        return;
    }

    for (Nodo* hijo : nodo->hijos) {
        if (!visitados[hijo->datos.first - 'A']) {
            dfs(hijo, visitados, camino, nodoIr);
            if (!camino.empty() && camino.back() == nodoIr) {
                return;
            }
        }
    }
    camino.pop_back();
}

int main() {
    ifstream archivo("matriz.txt");
    if (!archivo.is_open()) {
        cout << "Error al leer el archivo" << endl;
        return 1;
    }

    int n;
    archivo >> n;

    if (n == 0) {
        cout << "No hay matriz" << endl;
        return 0;
    }

    vector<vector<int>> matriz(n, vector<int>(n,0));

    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            char valor;
            archivo >> matriz[i][j];
            if (archivo.peek() == ',') {
                archivo.ignore();
            }
        }
    }
    archivo.close();

    cout << "Los nodos son: ";
    for (int i=0; i<n; i++) {
        char caracter = i+65;
        if (i < n-1) {
            cout << caracter << ", ";
        } else {
            cout << caracter << endl;
        }

    }

    char escojer;
    int nodoIr;
    do {
        cout << "Introducir nodo al que se quiere ir (letras en mayuscula)"<<endl;
        cin >> escojer;
        nodoIr = escojer - 'A';
        if (nodoIr >= 0 && nodoIr < n) {
            break;
        }
    } while (true);
    
    vector<int> distancias(n, INT_MAX);
    vector<Nodo*> nodos(n, nullptr);

    nodos[0] = new Nodo('A', 0);
    dijkstra(matriz, distancias, nodos);

    if (distancias[nodoIr] == INT_MAX) {
        cout << "No existe una ruta desde A hasta " << escojer;
        return 0;
    }

    cout << "La distancia al camino mas corto de A a " << escojer << " es: " << distancias[nodoIr] << endl;

    vector<bool> visitados(n, false);
    vector<char> camino;
    dfs(nodos[0], visitados, camino, escojer);

    if (!camino.empty()) {
        cout << "El camino mas corto es: ";
        for (int i=0; i<camino.size(); i++) {
            cout << camino[i];
            if (i < camino.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;
    } else {
        cout << "No hay un camino al nodo" << endl;
    }

    return 0;
}