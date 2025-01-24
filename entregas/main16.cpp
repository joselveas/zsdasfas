//MAXIMILIANO ALFREDO PASTEN NAYEM
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <queue>

using namespace std;

class Nodo {
public:
    int id;
    int distancia;

    Nodo(int _id, int _distancia) {
        id = _id;
        distancia = _distancia;
    }
};

bool abrirArchivo(string& ruta, vector<vector<int>>& matriz, int& cantNodos) {
    ifstream txt(ruta);
    if (!txt.is_open()) {
        cout << "No se encuentra el archivo" << endl;
        return false;
    }

    string primeraLinea;
    if (!getline(txt, primeraLinea) || primeraLinea.empty()) {
        cout << "El archivo esta vacio" << endl;
        return false;
    }
    try {
        cantNodos = stoi(primeraLinea);
    } 
    catch (invalid_argument&) {
        cout << "la primera linea no es un numero valido" << endl;
        return false;
    }
    if (cantNodos <= 0) {
        cout << "Error - El numero de nodos debe ser mayor a 0" << endl;
        return false;
    }

    matriz.resize(cantNodos, vector<int>(cantNodos, 0));

    string linea;
    int i = 0;

    while (getline(txt, linea)) {
        if (i >= cantNodos) {
            cout << "Error - mas filas de las esperadas" << endl;
            return false;
        }

        stringstream ss(linea);
        string valor;
        int j = 0;

        while (getline(ss, valor, ',')) {
            if (j >= cantNodos) {
                cout << "Error - mas columnas de las esperadas en la fila " << i + 1 << endl;
                return false;
            }

            int peso;
            try {
                peso = stoi(valor);
            } 
            catch (invalid_argument&) {
                cout << "Error - '" << valor << "' no es un numero valido" << endl;
                return false;
            }

            if (peso < 0) {
                cout << "Error - Hay un valor/coste negativo" << endl;
                return false;
            }

            matriz[i][j] = peso;
            ++j;
        }

        if (j != cantNodos) {
            cout << "Error - la fila " << i + 1 << " no tiene el numero correcto de columnas" << endl;
            return false;
        }

        ++i;
    }

    if (i != cantNodos) {
        cout << "Error - El numero de filas no coincide con el numero de nodos" << endl;
        return false;
    }

    txt.close();
    for (int i=0;i<cantNodos;i++) {
        for (int j=0;j<cantNodos;j++) {
            cout << matriz[i][j]<< " ";
        }
        cout << endl;
    }
    for (int i = 0; i < cantNodos; ++i) {
        cout << (char)('A' + i) << " ";
    }
    cout << endl;

    return true;
}

char pedirLetra(int cantNodos) {
    char letra;
    cout << "Ingrese el nodo Destino: ";
    cin >> letra;

    while (letra < 'A' || letra >= 'A' + cantNodos) {
        cout << "Error - Ingrese una letra valida entre A y " << (char)('A' + cantNodos - 1) << ": ";
        cin >> letra;
    }
    return letra;
}

void dijkstra(const vector<vector<int>>& matriz, int nodoInicio, int nodoFinal, int cantNodos) {
    if (nodoInicio == nodoFinal) {
        cout << "El nodo de inicio y el nodo de destino son el mismo: " << endl;
        cout << "El costo mas corto es 0." << endl;
        cout << "El camino mas corto es: No se mueve"  << endl;
        return;
    }

    vector<int> distancias(cantNodos, 101); //100 se toma como el valor mas grande 
                                            //por ende 101 se toma como infinito
    vector<int> padres(cantNodos, -1);
    vector<bool> visitados(cantNodos, false);

    distancias[nodoInicio] = 0;

    queue<Nodo> q;
    q.push(Nodo(nodoInicio, 0));

    while (!q.empty()) {
        Nodo actual = q.front();
        q.pop();

        if (visitados[actual.id]) continue;
        visitados[actual.id] = true;

        for (int vecino = 0; vecino < cantNodos; ++vecino) {
            int peso = matriz[actual.id][vecino];
            if (peso > 0) {
                int nuevaDistancia = distancias[actual.id] + peso;
                if (nuevaDistancia < distancias[vecino]) {
                    distancias[vecino] = nuevaDistancia;
                    padres[vecino] = actual.id;
                    q.push(Nodo(vecino, nuevaDistancia));
                }
            }
        }
    }

    if (distancias[nodoFinal] == 101) {
        cout << "No hay un camino desde " << (char)('A' + nodoInicio)
             << " hasta " << (char)('A' + nodoFinal)  << endl;
    } else {
        cout << "El costo mas corto desde " << (char)('A' + nodoInicio)
             << " hasta " << (char)('A' + nodoFinal) << " es " << distancias[nodoFinal]  << endl;

        vector<int> camino;
        for (int v = nodoFinal; v != -1; v = padres[v]) {
            camino.push_back(v);
        }

        cout << "El camino mas corto es: ";
        for (int i = camino.size() - 1; i >= 0; --i) {
            cout << (char)('A' + camino[i]) << " ";
        }
        cout << endl;
    }
}

int main() {
    vector<vector<int>> matriz;
    int cantNodos;

    string ubiTxt = "/workspaces/TareaEstructura/matriz.txt";

    if (abrirArchivo(ubiTxt, matriz, cantNodos)) {
        char letraFinal = pedirLetra(cantNodos);
        cout << "Nodo destino: " << letraFinal << endl;

        int nodoInicio = 0;
        int nodoFinal = letraFinal - 'A';
        dijkstra(matriz, nodoInicio, nodoFinal, cantNodos);
        
    }
    
    return 0;

}