//ABRAHAM ALEJANDRO SEPULVEDA CABALLERO
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <utility>
#include <queue>
#include <stack>
using namespace std;

class Nodo{
   public:
    char letra;
    int peso;
    vector<Nodo*> hijos;
    Nodo(char letra, int peso) : letra(letra), peso(peso){
    }
    
};


vector<int> dijkstra(vector<vector<int>> &matriz){
    int n = matriz.size();
    vector<Nodo*> arbol(n);
    for(int i = 0; i < n; i++){
        arbol[i] = nullptr;
    }
    vector<int> caminoCorto(n,99999);
    vector<int> nodos(n,-1);
    caminoCorto[0] = 0;
    arbol[0] = new Nodo(65, 0);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(matriz[i][j] != 0){
                int peso = matriz[i][j];  
                if(caminoCorto[i] + peso < caminoCorto[j]){
                    caminoCorto[j] = caminoCorto[i] + peso;
                    nodos[j] = i;
                    if (!arbol[j]) {
                        arbol[j] = new Nodo(j+65, caminoCorto[j]);
                    }
                    arbol[i]->hijos.push_back(arbol[j]);              
                }
            }      
        }
    }
    return nodos;
}

vector<int> dijkstra2(vector<vector<int>> &matriz){
    int n = matriz.size();
    vector<int> caminoCorto(n,99999);
    caminoCorto[0] = 0;
    for(int i = 0; i < n; i++){
        for(int j = 1; j < n; j++){
            if(matriz[i][j] != 0){
                int peso = matriz[i][j];  
                if(caminoCorto[i] + peso < caminoCorto[j]){
                    caminoCorto[j] = caminoCorto[i] + peso;                         
                }
            }      
        }
    }
    return caminoCorto;
}

void imprimirCamino(vector<int>& nodos, int nodoFinal) {
    if(nodos.size() <= nodoFinal){
        cout<<"No se encontró el nodo"<<endl;
        return;
    }
    stack<int> camino;
    int actual = nodoFinal;

    while(actual != -1){
        camino.push(actual);
        actual = nodos[actual];
    }
    while(!camino.empty()){
        char nodo = camino.top() + 65;
        cout<<nodo;
        camino.pop();
        if(!camino.empty()){
            cout<<"->";
        }
    }
    cout<<endl;
}

void imprimirNodos(int n){
    queue<char> nodos;
    cout<<"Los nodos presentes son: ";
    for(int i = 0; i < n; i++){
        nodos.push(i+65);
    }
    while(!nodos.empty()){
        cout<<nodos.front()<<" ";
        nodos.pop();
    }
}

int main(){
    ifstream archivo("matrizEjemplo.txt");
    if(!archivo){
        cout<<"No existe el archivo"<<endl;
        return 1;
    }
    string primeraLinea;
    getline(archivo,primeraLinea);
    int n = stoi(primeraLinea);

    vector<vector<int>> matrizAdy(n,vector<int>(n));
    int contador = 0;
    string linea;

    for(int i = 0; i < n; i++){
        getline(archivo,linea);
        stringstream ss(linea);
        string parte;
        int j = 0;
        while(getline(ss,parte,',')){
            int peso = stoi(parte);
            matrizAdy[i][j] = peso;
            if(i == j){
                //pa los ciclos
                matrizAdy[i][j] = 0;
            }
            j++;
        }
    }

    imprimirNodos(n);
    cout<<endl;
    //este dijkstra es para construir el arbol
    vector<int> nodosCaminoCorto = dijkstra(matrizAdy);
    //este dijkstra es para guardar el vector de los caminosCortos
    vector<int> caminoCorto = dijkstra2(matrizAdy);
    
    
    char nodoFin;
    while(true){
        cout<<"Ponga el nodo a llegar(Mayusculas): ";
        cin>>nodoFin;
        if(nodoFin < 65 || nodoFin > 90){
            cout<<"Carácter inválido"<<endl;
        }else{
            break;
        }
    }
            
    int nodoFinInt = nodoFin - 65;

    for(int i = 0; i < caminoCorto.size(); i++){
        if(nodoFinInt == i){
            cout<<"El valor total del camino más corto: "<<caminoCorto[i]<<endl;
        }
    }
    
   imprimirCamino(nodosCaminoCorto,nodoFinInt);

    return 0;
}