//BENJAMIN TORRES INOSTROZA
#include <vector>
#include <iostream>
#include <queue>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include<climits>

using namespace std;

int setCamino(vector<int> &caminos, vector<bool> &visitados){   //este subprograma encuentra el camino con menor peso dentro de la fila (ya que en el grafico es la fila)
    int menor = INT_MAX;
    int indce = -1;
    for (int i = 0; i < caminos.size(); i++){
         if(!visitados[i] && caminos[i] < menor){
            menor = caminos[i];
            indce = i;
         }
    }
    if (indce >= 0 && indce < caminos.size()){
        
        return indce;
    } else {
        return -1;
    }
    
}


stack<int> dijkstra(vector<vector<int>> matriz, int nodo, int objetivo, int &dist) {
    int n = matriz.size();

    vector<int> caminos(n, INT_MAX);

    caminos[nodo] = 0;    

    vector<bool> visitados(n, false);

    stack<int> camino;               
    stack<int> invertir;


    vector<int> anteriores(n, -1);       

    int fila;

    
    do {
        fila = setCamino(caminos, visitados); 

        if (fila != -1) {
            visitados[fila] = true;

            for (int i = 0; i < matriz[fila].size(); i++) {
                
            if (!visitados[i] && matriz[fila][i]!= 0 && caminos[fila]+matriz[fila][i] < caminos[i]) {


                caminos[i] = caminos[fila] + matriz[fila][i];

                anteriores[i] = fila; 
                camino.push(i);  

                }

                
                if (fila == objetivo) {

                    int actual = objetivo;
                    dist = caminos[actual];

                    while (actual != -1) {

                        invertir.push(actual);
                        
                        actual = anteriores[actual]; 
                    }
                    return invertir; 
                }
        }
    }

   
    } while (fila != -1);

    
    return invertir;
}

void imprimirMatriz(vector<vector<int>> matriz){ //funcion usada para comprobar que el programa lee el archivo
    for (int i = 0; i <matriz.size() ; i++){
        for (int j = 0; j <matriz[0].size() ; j++){
            cout << matriz[i][j] << ", ";
        }
        cout << endl;
    }
}

char numeroALetra(int num, int n) {
    if (num >= 0 && num <= 26) {
        return 'A' + num;
    } else {
        cout << "El número debe estar entre 0 y "<< n-1 << endl;
        return ' ';
    }
}

vector<vector<int>> leerArchivoAMatriz(string nombreArchivo){
    ifstream archivo(nombreArchivo);
    string linea;
    vector<vector<int>> matriz;

    if (getline(archivo,linea)){
        
        int n = stoi(linea); 

        if (n > 26){
            cout << "La cantidad de nodos excede el límite (max. 26)" << endl;
            return matriz;
        } else if (n < 0){
            cout << "Cantidad de nodos no válida" << endl;
            return matriz;
        }

        while(getline(archivo,linea)){
            vector<int> fila;
            stringstream separador(linea);
            string valorPos;

            while (getline(separador, valorPos, ',')) {

                fila.push_back(stoi(valorPos));  // convertir cada valor a entero según cada cajita de la matriz
            }

            matriz.push_back(fila);
        }
    } else {
        cout << "El archivo está vacío" << endl;
        
    }
    return matriz;
}

int main(){

    vector<vector<int>> matriz = leerArchivoAMatriz("matriz.txt");

    if (matriz.empty()){
        return 0;
    }

    int n = matriz.size(); //cant de nodos

    cout << "Ingrese el número de alguno de estos nodos para encontrar el camino más corto a él\n" ;

    for (int i = 0; i < n-1; i++){   //interfaz inicial 
        cout << numeroALetra(i,n) << "("<< i <<")" << " | ";
    }
    cout << numeroALetra(n-1,n) << "("<< n-1 <<")\n" << endl;


    int objetivo = n;  //variable del numero del nodo que se quiere guardar
    
    while (objetivo < 0 || objetivo >=n){
        cin >> objetivo;
        
        if (objetivo < 0 || objetivo >=n){
            cout << "\nIngrese un número desde el 0 al " << n-1 << endl;
        }
    }

    int dist;
    stack<int> menorCamino = dijkstra(matriz,0, objetivo,dist);
    cout << "El menor camino para " <<numeroALetra(objetivo, n);
    if (menorCamino.size() > 0){
        cout <<" es: \n" << endl;

        cout << numeroALetra(menorCamino.top(),n);
        menorCamino.pop();

        while (!menorCamino.empty())
    {
        cout << " -> " << numeroALetra(menorCamino.top(),n);
        menorCamino.pop();
    }
    cout <<"\nCon una distancia de " << dist << "\n" << endl;
    } else {
        cout << " no existe (es cero)." << endl;
    }
    

    return 0;
}