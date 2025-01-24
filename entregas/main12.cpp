//itzel acuna
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <limits>
#include <cctype>
#include <queue>
#include <stack>


using namespace std; 

/*Vamos a leer el archivo para crear la matriz yipie*/
vector<vector<int>> readFile (string filee){
    ifstream file(filee);
    if(!file){
        cout << "No se encontró el archivo \n";
        return {}; 
    }

    int size; 
    string line; 
    getline(file, line); 

    stringstream convertir(line); 
    convertir >> size;

    if(size <= 0){
        cout <<"El tamaño de la matriz es inváliddo";
        return{};
    }

    vector<vector<int>> matrix(size, vector<int>(size));

    for (int i = 0; i < size; i++) {
        getline(file, line); 
        stringstream lineS(line); 
        string n; 

        for (int j = 0; j < size; j++) {
            getline(lineS,n, ',' );
            matrix [i][j]= stoi(n);
        }
    }

    return matrix;
}

/*EL algoritmo de dijkstra pa saber el numerito del camino más corto jiji*/
vector<int> dijkstra (vector<vector<int>> matrix, int start, vector<int> & parent){
    int size= matrix.size();
    vector <int> distance( size, numeric_limits<int>::max());
    vector <bool> visit (size, false);
    parent= vector <int> (size, -1);

    distance[start]= 0; 

    for (int i=0; i< size; i++){
        int min= numeric_limits<int>::max();
        int actual= -1; 

        for (int j=0; j < size; j++){
            if(!visit[j] && distance [j] < min){
                min= distance [j];
                actual= j; 

            }
        }

        if (actual== -1) break;

        visit[actual]= true; 

        for ( int a=0; a< size; a++){
            if(matrix[actual][a] > 0 && !visit[a]){
                int newDistance= distance[actual] + matrix [actual][a];
                if(newDistance < distance[a]){
                    distance[a]= newDistance; 
                    parent[a]= actual;
                }
            }
        }
    }

    return distance; 

}

/*Arbol con cosas de bfs y esas csas que vimos al final pa la segunda prueba*/

class Nodo{
    public: 
    int letter; 
    int value; 
    int parent; 

    Nodo(int l, int v, int p){
        this -> letter= l; 
        this-> value= v;
        this -> parent= p; 
    }

}; 

/*
vector<Nodo*> bfs(vector<vector<int>>matrix, int start, int final){
    int size= matrix.size();
    vector<bool> visit(size,false); 
    queue<int> queue; 
    vector<Nodo*> nodo(size,nullptr); 

    visit[start]= true;
    queue.push(start);
    nodo[start]= new Nodo(start, 0, -1);

    while (!queue.empty()){
        int actual= queue.front();
        queue.pop();

        if (actual == final) break; 

        for( int i= 0; i < size; i++){
            if(matrix[actual][i]> 0 && !visit [i]){
                visit[i]= true;
                queue.push(i);

                int newValue= nodo[actual]-> value + matrix[actual][i];
                nodo [i]= new Nodo(i,newValue,actual);
            }
        }
    }

    return nodo; 
}*/

void shortestPath (vector<int>& parent, int final){
    stack<char> path;
    /*int value= nodo[final]->value;*/
    int actual= final; 

    while(actual != -1){
        char nChar= 'A'+ actual; 
        path.push(nChar);
        actual= parent[actual]; 

    }

    cout <<"El camino a seguir: ";

    while(!path.empty()){
        cout << path.top();
        path.pop();

        if (!path.empty()){
            cout << "->";
        }
    }

    cout<< endl; 
}

int main(){

    string file= "texto.txt"; 
    vector<vector<int>> matrix= readFile(file); 

    /*imprimo la matriz que aparece en el txt pa verificar*/
    for (const auto& row : matrix) {
        for (int n : row) {
            cout << n << " ";
        }
        cout << endl;
    }

    /*Implemento la parte de letras y cosas de dijkstra :v*/
    char input; 
    cout << "\n Ingrese el nodo al cual quiere llegar: ";
    cin >> input; 
    input= toupper(input);

    int find = input - 'A'; 

    /*control de error*/
    if(find < 0 || find >= matrix.size()){
        cout <<"Nodo inválido \n";
        return 1; 
    }

   /* vector <Nodo*> nodo = bfs(matrix, 0, find); */
    vector <int> parent; 
    vector<int> distance= dijkstra (matrix, 0, parent);

    if(distance [find]  == numeric_limits<int>::max()){
        cout << "No hay camino \n";
    }else{
        cout <<" La distancia es: " << distance [find] << endl; 
        /*vector <Nodo*> nodo = bfs(matrix, 0, find); */
        /*Hay que indicar el camino para llegar al nodo solicitado , llamo al sP*/
        shortestPath(parent,find);
        
    }

    
    
    /* redii */
    return 0;
}