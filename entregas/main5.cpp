//Clerians Marquez Carvajal
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <queue>
#include <climits>

using namespace std;

class Nodo {//clase nodo
public:
    pair<char, int> nombres;
    vector<pair<Nodo*, int>> vecinos;

    Nodo(char nombre, int peso) {
        nombres = {nombre, peso};
    }
};

Nodo* armarArbol(int n, vector<vector<int>>& matriz, vector<Nodo*>& lista) { //construye el arbol 
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matriz[i][j] > 0) {
                Nodo* hijo = lista[j];
                int peso = matriz[i][j];
                lista[i]->vecinos.push_back({hijo, peso});
            }
        }
    }
    return lista[0];
}

pair<vector<char>, int> dijkstra(Nodo* inicio, Nodo* fin, int n) {//algoritmo dijkstra
    vector<int> distancia(n, 1000000);
    vector<Nodo*> visitados(n, nullptr);
    distancia[inicio->nombres.first - 'A'] = 0;
    
    queue<Nodo*> cola;
    cola.push(inicio);

    while (!cola.empty()) {
        Nodo* aux = cola.front();
        cola.pop();

        for (int i = 0; i < aux->vecinos.size(); i++) {
            Nodo* siguiente = aux->vecinos[i].first;
            int peso = aux->vecinos[i].second;
            int indiceSiguiente = siguiente->nombres.first - 'A';

            if (distancia[aux->nombres.first - 'A'] + peso < distancia[indiceSiguiente]) {
                distancia[indiceSiguiente] = distancia[aux->nombres.first - 'A'] + peso;
                visitados[indiceSiguiente] = aux;
                cola.push(siguiente);
            }
        }
    }

    vector<char> recorrido;
    for (Nodo* a = fin; a != nullptr; a = visitados[a->nombres.first - 'A']) {
        recorrido.push_back(a->nombres.first);
    }

    vector<char> recorridoSeverla;
    for (int i = recorrido.size() - 1; i >= 0; i--) {
        recorridoSeverla.push_back(recorrido[i]);
    }

    int total = distancia[fin->nombres.first - 'A'];
    return {recorridoSeverla, total};//devulve el camino y la distancia 
}

int main() {

    string nombre;
    ifstream leerArch;

    while (true) {//ciclo por si se ingresa mal el nombre del archivo
        cout << "Ingrese nombre del archivo (ej: matriz.txt): " << endl;
        cin >> nombre;
        leerArch.open(nombre);

        if (leerArch.is_open()) {
            break;
        } else {
            cerr << "No se encontró el archivo. Intente otra vez." << endl;
            leerArch.clear();
        }
    }

    int n;
    leerArch >> n;
    leerArch.ignore();

    vector<vector<int>> matriz(n, vector<int>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j < n - 1) {
                leerArch >> matriz[i][j];
                leerArch.ignore(1, ',');
            } else {
                leerArch >> matriz[i][j];
            }
        }
    }
    leerArch.close();

    vector<Nodo*> lista;
    cout << "Nodos que se pueden visitar: " << endl;
    for (int i = 0; i < n; i++) {
        char nodo = 'A' + i;
        cout<< nodo << " ";
        lista.push_back(new Nodo(nodo, 0));//se crea los nodos
    }cout << endl;

    Nodo* arbol = armarArbol(n, matriz, lista);//crea el arbol con la matriz y la lista

    char final;
    int indice;

    while(true){//ciclo por si se ingresa una letra fuera del rango
        cout << "Donde vamos? (El camino empieza en A): " << endl;
        cin >> final;
        final = toupper(final);
        indice = final - 'A';

        if (indice < n && indice >= 0) {
            break;
        } else {
            cout << "Destino no válido" << endl;
        }
    }

    pair<vector<char>, int> resultado = dijkstra(lista[0], lista[indice], n);//llama la funcion dijkstra
    vector<char> camino = resultado.first;
    int total = resultado.second;

    cout << "Tiempo de viaje: " << total << endl;//print peso

    cout << "Camino más corto de A a " << final << ": ";
    for (int i = 0; i < camino.size(); i++) {//print camino 
        cout << camino[i] << " ";
    }
    cout << endl;
        
    return 0;
}