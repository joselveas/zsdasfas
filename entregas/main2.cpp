//DIEGO IGNACIO ADAOS ALVAREZ
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

class Nodo{
    public:
        pair<char,int> datos;
        vector<Nodo*> hijos;

        Nodo(char nodo,int distancia):datos(nodo,distancia){}
};
vector<vector<int>> leerArchivo(string ruta){
    ifstream archivo(ruta);
    if(!archivo){
        cerr << "No se pudo abrir el archivo."<<endl;
        exit(1);
    }
    int n;
    archivo >>n;
    archivo.ignore();

    vector<vector<int>> grafo(n,vector<int>(n));
    string linea;

    for(int i = 0;i<n;++i){
        getline(archivo, linea);
        stringstream ss(linea);

        for(int j = 0;j < n;++j){
            string valor;
            getline(ss, valor, ',');
            grafo[i][j] = stoi(valor);
        }
    }
    archivo.close();
    return grafo;
}
vector<int> dijkstra(vector<vector<int>> &grafo,vector<int> &padres,Nodo* &raiz){
    int n = grafo.size();
    int max = 999999;
    vector<int> distancias(n,max);
    padres.resize(n,-1);
    vector<bool> visitados(n,false);

    distancias[0] = 0;
    raiz = new Nodo('A',0);
    vector<Nodo*> nodos(n,nullptr);
    nodos[0] = raiz;

    for(int i = 0;i<n;++i){
        int actualMin = -1;
        for(int j = 0;j<n;++j){
            if(!visitados[j] && (actualMin == -1 || distancias[j]<distancias[actualMin])){
                actualMin = j;
            }
        }
        if( distancias[actualMin] == max){
            break;
        }

        visitados[actualMin] = true;
        for(int j = 0; j < n; ++j){
            int vecino = j;
            if (grafo[actualMin][vecino] && distancias[actualMin]+grafo[actualMin][vecino]<distancias[vecino]){
                distancias[vecino] = distancias[actualMin]+grafo[actualMin][vecino];
                padres[vecino] = actualMin;
                
                Nodo* nuevoNodo = new Nodo('A'+vecino,distancias[vecino]);
                nodos[vecino] = nuevoNodo;
                nodos[actualMin]->hijos.push_back(nuevoNodo);
            }

        }

    }
    return distancias;
}
bool dfsArbol(Nodo* nodo, char destino, string &ruta){
    ruta += nodo->datos.first;
    if(nodo->datos.first == destino){
        return true;
    }
    for(Nodo* hijo : nodo->hijos){
        if(dfsArbol(hijo,destino,ruta)){
            return true;
        }
    }
    ruta.pop_back();
    return false;
}

int main() {
    vector<char> letrasNodos ={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    int max = 999999;
    vector<vector<int>> grafo = leerArchivo("matriz.txt");

    cout<<"Matriz de adyacencia cargada:"<<endl;
    for(int i = 0;i<grafo.size();i++){
        for(int j = 0;j<grafo.size();j++){

            cout<<grafo[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;

    cout<<"Nodos cargados: ";
    for(int i = 0;i<grafo.size();i++){
        cout<<letrasNodos[i]<<" ";
    }
    cout<<endl;

    char nodoDestino;
    cout<<"A que nodo desea llegar?";
    cin>>nodoDestino;
    int destino = toupper(nodoDestino)-'A';

    vector<int> padres;
    Nodo* raiz = nullptr;
    vector<int> distancias =dijkstra(grafo,padres,raiz);

    if (distancias[destino] == max) {
        cout<<"No se puede llegar a ese nodo desde A."<<endl;
        return 0;
    }

    cout<<"longitud del camino mas corto: "<<distancias[destino]<<endl;

    string ruta;
    if(dfsArbol(raiz, 'A' + destino, ruta)){
        cout<<"Ruta hacia el nodo "<<nodoDestino<<": "<<ruta<<endl;
    }else{
        cout<<"No se encontro la ruta hacia el nodo "<<nodoDestino<<endl;
    }

    return 0;
}