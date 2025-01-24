//Denzel Martin Delgado Urquieta
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <climits>
#include <fstream>
#include <sstream>
using namespace std;

//Clase del arbol que guardara la ruta mas corta para cada nodo
class Arbol {
    public:
        pair<char, int> nodo;
        vector<Arbol*> siguiente;
        Arbol(char n, int pesoAcumulado) {
            nodo.first = n;
            nodo.second = pesoAcumulado;
        }
};

//Lee el archivo y rellena la matriz
vector<vector<int>> leerArchivo(string nombreArchivo) {
    vector<vector<int>> vacio;
    ifstream archivo(nombreArchivo);
    if(!archivo.is_open()) {
        cout << "No se ha logrado abrir el archivo o no existe" << endl;
        return vacio;
    } else if(archivo.peek() == EOF) {
        cout << "El archivo esta vacio" << endl;
        return vacio;
    }

    int numNodos;
    string linea;
    if(getline(archivo, linea)) {
        stringstream ss(linea);
        if(!(ss >> numNodos) || !ss.eof()) {
            cout << "El formato del archivo no es correcto" << endl;
            return vacio;
        }
    }
    
    if(numNodos < 1) {
        cout << "No es posible trabajar con esa cantidad de nodos. Minimo: 1" << endl;
        return vacio;
    } else if(numNodos > 26) {
        cout << "No es posible trabajar con esa cantidad de nodos. Maximo: 26" << endl;
        return vacio;
    }
    vector<vector<int>> matriz(numNodos, vector<int>(numNodos, 0));

    int lineas = 0;
    while(getline(archivo, linea)) {
        if(lineas >= numNodos) {
            cout << "El formato de la matriz no concuerda con la cantidad de nodos. Tiene fila(s) extra(s) o lineas en blanco" << endl;
            return vacio;
        }
        stringstream ss(linea);
        for(int i = 0; i < numNodos; i++) {
            if(!(ss >> matriz[lineas][i])) {
                cout << "Hay un error de formato en la matriz. Tal vez contenga datos no enteros o espacios en blanco: " << linea << endl;
                return vacio;
            }
            ss.ignore();
            if(matriz[lineas][i] < 0) {
                cout << "No es posible trabajar con pesos negativos" << endl;
                return vacio;
            }
        }
        if(!ss.eof()) {
            cout << "Hay un error de formato en la matriz ubicado al final de una fila (tal vez tenga espacios en blanco adicionales): " << linea << endl;
            return vacio;
        }
        
        lineas++;
    } archivo.close();
    if(lineas == 0 || lineas != numNodos) {
        cout << "Cantidad de entradas incompatibles o no existen datos para la matriz" << endl;
        return vacio;
    } 
    return matriz;
}

//Imprime todos los nodos presentes en la matriz
void nodosLeidos(vector<vector<int>> matriz) {
    cout << "Nodos leidos: ";
    char letra = 'A';
    for(int i = 0; i < matriz.size(); i++) { 
        cout << letra << " ";
        letra = letra + 1;
    } cout << endl;
}

//Realiza una busqueda nivel por nivel hasta encontrar el nodo inicial con su respectiva 
//ruta en base a la arista y la distancia calculada por dijkstra
Arbol* busquedaDeNodoInicial(Arbol* head, char nodoI, int pesoArista, int pesoDistancia) {
    queue<Arbol*> cola;
    cola.push(head);
    int pesoActual = 0;
    while(!cola.empty()) {
        Arbol* actual = cola.front();
        cola.pop();
        pesoActual = actual -> nodo.second;
        if(actual -> nodo.first == nodoI && (pesoActual + pesoArista) == pesoDistancia) return actual;
        for(int i = 0; i < actual -> siguiente.size(); i++) {
            cola.push(actual -> siguiente[i]);
        }
    }
    return nullptr;
}

//La funcion agrega al arbol el hijo del nodo inicial correspondiente
void agregarArbol(int pesoDistancia, int nodoI, int nodoD, int pesoArista, Arbol*& head) {
    nodoI = nodoI + 65;
    nodoD = nodoD + 65;
    char inicial = nodoI;
    char destino = nodoD;
    Arbol* nuevoHijo = nullptr;    
    Arbol* nodoActual = busquedaDeNodoInicial(head, inicial, pesoArista, pesoDistancia);
    if(nodoActual != nullptr) {
        nuevoHijo = new Arbol(destino, pesoDistancia);
        nodoActual -> siguiente.push_back(nuevoHijo); 
    } 
}

//El algoritmo calcula la distancia mas corta de todos los nodos a partir del nodo A, 
//cuyo calculo de distancia se envia al metodo agregarArbol para su construccion
void dijkstra(vector<int>& distancia, vector<bool>& visitado, vector<vector<int>> matriz, 
                int tamano, Arbol*& head) {
    distancia[0] = 0;
    for(int nodos = 0; nodos < tamano; nodos++) {
        int nodoActual = -1;
        
        for(int nodo = 0; nodo < tamano; nodo++) {
            if(!visitado[nodo] && (nodoActual == -1 || distancia[nodo] < distancia[nodoActual])) {
                nodoActual = nodo;
            }
        }

        if(distancia[nodoActual] == INT_MAX) {
            break;
        }
        visitado[nodoActual] = true;
        for(int vecino = 0; vecino < tamano; vecino++) {
            if(matriz[nodoActual][vecino] != 0 && !visitado[vecino]) {
                int dist = distancia[nodoActual] + matriz[nodoActual][vecino];
                if(dist < distancia[vecino]) {
                    distancia[vecino] = dist;
                    agregarArbol(dist, nodoActual, vecino, matriz[nodoActual][vecino], head);
                }
            }
        }
    }   

}

//Realiza una busqueda en profundidad, la cual, imprime la informacion del nodo destino
void imprimirRutaDestino(Arbol* head, char nodoD, int menor) {
    vector<char> ruta;
    stack<Arbol*> pila;
    stack<int> indices;

    pila.push(head);
    indices.push(0);

    while(!pila.empty()) {
        Arbol* actual = pila.top();
        int indice = indices.top();

        if(actual -> nodo.first == nodoD && actual -> nodo.second == menor) {
            ruta.push_back(actual -> nodo.first);
            break;
        }

        if(indice < actual -> siguiente.size()) {
            int auxIndice = indices.top();
            indices.pop();
            auxIndice++;

            indices.push(auxIndice);
            pila.push(actual -> siguiente[indice]);
            indices.push(0);
            ruta.push_back(actual -> nodo.first);
        } else {
            pila.pop();
            indices.pop();

            if(ruta.size() > 0) {
                ruta.pop_back();
            }
        }
    }

    cout << "Distancia: " << menor << endl;
    cout << "Ruta: ";
    for(int j = 0; j < ruta.size(); j++) {
        if((ruta.size() - 1) == j) {
            cout << ruta[j];
        } else {
            cout << ruta[j] << " -> ";
        }
    } cout << endl;
}

//Realiza una busqueda nivel por nivel, la cual, calcula el menor a partir del nodo destino
//presente en cada nodo inicial
int encontrarElMenor(Arbol* head, char nodoD) {
    queue<Arbol*> cola;
    cola.push(head);
    int menor = INT_MAX;
    while(!cola.empty()) {
        Arbol* actual = cola.front();
        cola.pop();
        if(actual -> nodo.first == nodoD) {
            if(actual -> nodo.second < menor) {
                menor = actual -> nodo.second;
            }
        }
        for(int i = 0; i < actual -> siguiente.size(); i++) {
            cola.push(actual -> siguiente[i]);
        }
    }
    if(menor == INT_MAX) return -1;
    return menor;
}
//----------------------------------------------------------------------------------------------
//                                          MAIN
//----------------------------------------------------------------------------------------------
int main() {
    cout << "====================================================================" << endl;
    cout << "Escribir la ruta del archivo txt:" << endl;
    string archivo;
    do {
        getline(cin, archivo);
        if(archivo.length() == 0) {
            cout << "Debe indicar una ruta" << endl;
        }
    } while(archivo.length() == 0);
    cout << "====================================================================" << endl;
    vector<vector<int>> matriz = leerArchivo(archivo);
    int tamano = matriz.size();
    Arbol* arbol = nullptr;
    if(tamano != 0) {
        arbol = new Arbol('A', 0);
        nodosLeidos(matriz);
        cout << "--------------------------------------------------------------------" << endl;
        vector<int> distancia(tamano, INT_MAX);
        vector<bool> visitado(tamano, false);
        dijkstra(distancia, visitado, matriz, tamano, arbol);
        string caracter;
        int ascii;
        cout << "Ingrese el nodo destino (o ingrese 'salir'):" << endl;
        while(true) {
            getline(cin, caracter);

            if(caracter == "salir"){
                cout << "Saliendo del programa..." << endl;
                break;
            } 
            
            if(caracter.length() == 0) {
                cout << "Ingrese un caracter" << endl;
            } else if(caracter.length() > 1) {
                cout << "Solo se permite un caracter" << endl;
            } else {
                ascii = caracter[0];
                if(ascii < 65 || ascii > (tamano + 64)) {
                    cout << "Debe ingresar uno de los nodos leidos exactamente como se muestra" << endl;
                } else {
                    char letra = ascii;
                    int menor = encontrarElMenor(arbol, letra);
                    if(menor == -1) {
                        cout << "El nodo al que se quiere llegar no es posible. Pruebe con otro." << endl;
                    } else {
                        imprimirRutaDestino(arbol, letra, menor);
                    }
                    cout << "********************************************************************" << endl;
                    cout << "Ingrese el nodo destino (o ingrese 'salir'):" << endl;
                }
            }
            cout << "********************************************************************" << endl;
        } 
    }
    return 0;
}