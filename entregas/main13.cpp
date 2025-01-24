//David Ignacio Pino Villivares
#include <iostream>
#include <vector>
#include <climits>
#include <fstream>
#include <sstream>
using namespace std;

class Nodo;

class Ady {
public:
    vector<pair<int, Nodo*>> conecta;
    Ady() {}
    void agregarConectores(int peso, Nodo* nodo) {conecta.push_back({peso, nodo});}
};

class Nodo {
public:
    char id;
    Nodo* sig;
    Ady* head;
    Nodo(char _id) {
        id = _id;
        sig = nullptr;
        head = new Ady();
    }
};

class Arbol {
public:
    pair<char, int> nodo;
    vector<Arbol*> caminos;

    Arbol(pair<char, int> nodo) : nodo(nodo) {}

    void agregarCamino(Arbol* camino) {caminos.push_back(camino);}

    void imprimirRecorrido() {
        if(nodo.first==65){cout << nodo.first << " (" << 0 << ")";}
        else{cout << nodo.first << " (" << nodo.second << ")";}

        for (int i = 0; i<caminos.size();i++) {
            cout << " -> ";
            caminos[i]->imprimirRecorrido();
        }       
    }
};

static Nodo* NodoGrafo = nullptr;
static int n;

void crearNodos(int n) {
    Nodo* aux = nullptr;
    for (int i = 0; i < n; i++) {
        char id = 65 + i;
        Nodo* nuevoNodo = new Nodo(id);
        if (NodoGrafo == nullptr) {
            NodoGrafo = nuevoNodo;
        } else {
            aux->sig = nuevoNodo;
        }
        aux = nuevoNodo;
    }
}
Nodo* buscarNodos(int k, int n) {
    Nodo* aux = NodoGrafo;
    for (int i = 0; i < n; i++) {
        if (aux == nullptr) {return nullptr;}
        if (k == i) {return aux;}
        aux = aux->sig;
    }
    return nullptr;
}
void leerArchivo(string nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo \"" << nombreArchivo << "\".\n";
        return;
    }
    string linea;
    getline(archivo, linea);
    if (linea == "0") {
        cout << "El archivo no contiene una matriz de adyacencia.\n";
        return;
    }
    n = stoi(linea);
    crearNodos(n);

    int i = 0;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        stringstream ss(linea);
        string parte;
        int j = 0;
        while (getline(ss, parte, ',')) {
            int peso = stoi(parte);
            if (i != j && peso > 0) {
                Nodo* nodoInicio = buscarNodos(i, n);
                Nodo* nodoDestino = buscarNodos(j, n);
                nodoInicio->head->agregarConectores(peso, nodoDestino);
            }
            j++;
        }
        i++;
    }

    archivo.close();
    cout << "Archivo procesado correctamente.\n";
}

void dijkstra(Nodo* nodoInicio, int n, vector<int>& v1, vector<int>& v2) {
    vector<bool> visitados(n, false);
    v1[nodoInicio->id - 65] = 0;

    for (int i = 0; i < n; i++) {
        int nodoActualInt = -1;
        int minDistancia = INT_MAX;
        for (int j = 0; j < n; j++) {
            if (!visitados[j] && v1[j] < minDistancia) {
                minDistancia = v1[j];
                nodoActualInt = j;
            }
        }
        if (nodoActualInt == -1) break;
        visitados[nodoActualInt] = true;
        Nodo* nodoActual = buscarNodos(nodoActualInt, n);

        for (int i = 0; i < nodoActual->head->conecta.size(); ++i) {
            int peso = nodoActual->head->conecta[i].first;
            Nodo* vecino = nodoActual->head->conecta[i].second;
            int vecinoInt = vecino->id - 'A';

            if (!visitados[vecinoInt] && v1[nodoActualInt] + peso < v1[vecinoInt]) {
                v1[vecinoInt] = v1[nodoActualInt] + peso;
                v2[vecinoInt] = nodoActualInt;
            }
        }
    }
}
void verCamino(int PuntoFinal,vector<int>& v2, vector<char>& camino) {
    if (PuntoFinal == -1) return;
    verCamino(v2[PuntoFinal], v2, camino);
    camino.push_back(PuntoFinal + 65);
}
void crearArbolImprimir(Nodo* nodoInicio, vector<int>& v1, int PuntoFinal, vector<char>& camino);
void procesarRuta()
{
    if (NodoGrafo != nullptr) {
        Nodo* nodoInicio = NodoGrafo;
        vector<int> v1(n, INT_MAX);
        vector<int> v2(n, -1);

        dijkstra(nodoInicio, n, v1, v2);
        
        char PuntoFinal;
        cout << "Ingrese el nodo que quieres llegar (Mayusculas): ";
        cin >> PuntoFinal;

        int destino = PuntoFinal - 65;
        if (destino >= 0 && destino < n) {
            if (v1[destino] == INT_MAX) {
                cout << "El nodo " << PuntoFinal << " no conecta con A\n";
            } else {
                cout << PuntoFinal << " " << v1[destino] << "\n";
                vector<char> camino;

                verCamino(destino, v2, camino);
                crearArbolImprimir(nodoInicio, v1, destino, camino);
                cout<<endl;
            }
        } 
        else {cout << "El nodo ingresado no existe compa o que esta en minuscula xD.\n";}
    }
}
void crearArbolImprimir(Nodo* nodoInicio, vector<int>& v1, int destino, vector<char>& camino) {
    Arbol* arbol = new Arbol({nodoInicio->id, 0});
    for (int i = 1; i < camino.size(); i++) {
        Arbol* rama = new Arbol({camino[i], v1[camino[i] - 65]});
        arbol->agregarCamino(rama);
    }
    arbol->imprimirRecorrido();
}
void imprimirTodosLosNodos() {
    Nodo* aux = NodoGrafo;
    cout << "Todos los nodos: ";
    while (aux->sig != nullptr) {
        cout << aux->id << " ";
        aux = aux->sig;
    }
    cout << aux->id << "\n";
}

int main() {
    
    leerArchivo("archivo.txt");
    imprimirTodosLosNodos();
    procesarRuta();

    return 0;
}