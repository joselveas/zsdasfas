//BENJAMIN EDUARDO ERAZO ORRIAN
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <queue>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max(); //creaci[on de INF para no usar 999999999999]

class Nodo{ //Clase de Nodo con constructor
public: 
    pair<char, int> nombreIndice;
    vector<pair<Nodo*, int>> vecinos;

    Nodo(char c, int index){
        nombreIndice = {c, index};
    }
};

void dijkstra(vector<Nodo*> &grafico, int inicio, int destino){ //Funcion de dijkstra 
    int n = grafico.size();
    vector<int> distancia(n,INF);
    vector<int> padre(n, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> colaPrioridad;

    distancia[inicio] = 0;
    colaPrioridad.push({0, inicio});

    while(!colaPrioridad.empty()){
        int u = colaPrioridad.top().second;
        colaPrioridad.pop();

        for(auto &vecino : grafico[u]->vecinos){
            int v = vecino.first->nombreIndice.second;
            int peso = vecino.second;

            if(distancia[u] + peso < distancia[v]){
                distancia[v] = distancia[u] + peso;
                padre[v] = u;;
                colaPrioridad.push({distancia[v], v});
            }
        }
    }

    if (distancia[destino] == INF){
        cout << "No hay un camino desde el nodo inicial hasta el nodo destino." << endl;
    } else{
        cout << "La distancia minima desde el nodo inicial hasta el nodo destino es: " << distancia[destino] << endl;
        cout << "El camino mas corto es: ";

        vector<int> camino;
        for (int v= destino; v != -1; v = padre[v]) {
            camino.push_back(v);
        }

        for(int i = camino.size() -1; i >=0; i--){
            cout << char('A' + camino[i]);
            if (i > 0){
                cout << "->";
            }
        }
        cout << endl;
    }

}


int main(){
    string nombreArch;
    cout << "Ingrese el nombre del archivo (ej: archivo.txt): ";
    cin >> nombreArch;
    fstream arch(nombreArch);

    if (arch.is_open()){ 
        int n;
        arch >>  n;
        arch.ignore();
        
        vector<Nodo*> grafico;

        for(int i = 0; i < n; i++){
            grafico.push_back(new Nodo('A' + i, i));
        }
        
        string linea; //leer archivo
        for(int i = 0; i<n;i++){
            getline(arch, linea);
            stringstream ss(linea);
            string num;

            for(int j = 0; j<n; j++){
                getline(ss, num, ',');
                int peso = stoi(num);
                if (peso != 0){
                    grafico[i]->vecinos.push_back({grafico[j], peso});
                }
            }
        }

        arch.close();

    cout << "Nodos existentes:" << endl;
    for(int i = 0; i < n; i++){
        cout << char('A' + i) << " ";
    }
    cout << endl;

    char nodo_destino;
    int dest;
    do{ //ciclo Do While para preguntar hasta que se inserte un nodo que existe
        cout << "(Con mayusculas las letras)" << endl;
        cout << "Ingrese hasta que nodo quiere llegar: ";
        cin >> nodo_destino;
        dest = nodo_destino - 'A';

        if(dest < 0 || dest >= n){
            cout << "Inserte un Nodo existente" << endl;
        }
    }while(dest < 0 || dest >= n);
    
    dijkstra(grafico, 0, dest);
    
    }else{
        cout << "No se pudo abrir el archivo. " << endl; //por si no funciona el archivo
    }


    return 0;
}