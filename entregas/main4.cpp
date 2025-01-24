//Felipe Blanco Pizarro
#include <iostream>
using namespace std;
#include <fstream>
#include <vector>

//cargar matriz del archivo txt y colocar los numeros en la matriz-----------------------------
bool cargarArchivoMatriz(vector<vector<int>>& matriz,int& tamanoMatriz,string nombreArchivo){
    ifstream archivoLeer(nombreArchivo);
    if(!archivoLeer.is_open()){
        cout<<"No se abrio el archivo"<<endl;
        return false;
    }
    archivoLeer>>tamanoMatriz;
    archivoLeer.ignore();
    if(tamanoMatriz > 0 && tamanoMatriz <= 26){
        matriz = vector<vector<int>>(tamanoMatriz,vector<int>(tamanoMatriz));
        string linea;
        int fila = 0;
        while(fila < tamanoMatriz && getline(archivoLeer,linea)){
            int columna = 0;
            string numero = "";
            for(int i = 0;i < linea.size();i++){
                if(i<linea.size() && linea[i] != ','){
                    numero += linea[i];
                }else{
                    matriz[fila][columna] = stoi(numero);
                    numero = "";
                    columna++;
                }
            
            }
            if(!numero.empty()){
                matriz[fila][columna] = stoi(numero);
            }
            fila++;
        }
        archivoLeer.close();
        return true;
    }else{
        cout<<"Tamaño de la matriz incorrecto. Modifique el txt"<<endl;
        return false;
    }
    
}//--------------------------------------------------------------------------------------------


//mostrar matriz para ver si funciona--------------------------------
void imprimirMatriz(vector<vector<int>>& matriz,int& tamanoMatriz){
    for(int i = 0;i<tamanoMatriz;i++){
        for(int j = 0;j<tamanoMatriz;j++){
            cout<<matriz[i][j]<<" ";
        }
        cout<<endl;
    }
}//------------------------------------------------------------------


//Clase Nodo--
class Nodo{
    public:
        char letra;
        int distancia;
    Nodo(char _letra){
        letra = _letra;
    }
};//----------


//Clase NodoArbol-
class NodoArbol{
    public:
        char letra;
        NodoArbol* raiz;
    NodoArbol(char _letra){
        letra = _letra;
        raiz = nullptr;
    }
};//--------------


//letras correspondientes a cant nodos--------
vector<Nodo*> letrasNodos(int tamanoMatriz){
    vector<char> letras = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    vector<Nodo*> nodosUsar(tamanoMatriz);
    cout<<"Nodos: ";
    for(int i = 0;i<tamanoMatriz;i++){
        nodosUsar[i] = new Nodo(letras[i]);
    }
    for(int j = 0;j<tamanoMatriz;j++){
        cout<<nodosUsar[j]->letra<<" ";
    }
    cout<<endl;
    return nodosUsar;
}//-------------------------------------------


//control de error letra de nodo a buscar---------------
Nodo* nodoEsta(vector<Nodo*> nodosUsar,char letraNodo){
    for(int i = 0;i<nodosUsar.size();i++){
        if(nodosUsar[i]->letra == letraNodo){
            return nodosUsar[i];
        }
    }
    return nullptr;
}//-----------------------------------------------------


//encontrar camino mas corto al nodo buscado------------------------------------------------------
int nodoMenorDistancia(vector<bool>& visitados,vector<int>& distanciaMasCorta,int tamanoMatriz){
    int menor = 9999;
    int nodoMDistancia = -1;

    for(int i = 0;i<tamanoMatriz;i++){
        if(!visitados[i] && distanciaMasCorta[i] < menor){
            menor = distanciaMasCorta[i];
            nodoMDistancia = i;
        }
    }
    return nodoMDistancia;
}//-----------------------------------------------------------------------------------------------


//preguntar por el nodo a buscar y buscar el camino mas corto--------------------------------
NodoArbol* dijkstra(vector<vector<int>>& matriz,int tamanoMatriz,vector<Nodo*>& nodosUsar){
    char letraNodo;
    cout<<"Escriba la letra del Nodo al que quiera llegar: ";
    cin>>letraNodo;
    Nodo* nodoAEncontrar = nodoEsta(nodosUsar,letraNodo);
    while(!nodoAEncontrar){
        cin.ignore();
        cout<<"Error. Ingrese correctamente la letra."<<endl;
        cout<<"Escriba la letra del Nodo al que quiera llegar: ";
        cin>>letraNodo;
        nodoAEncontrar = nodoEsta(nodosUsar,letraNodo);
    }
    vector<bool> visitados(tamanoMatriz);
    for(int i = 0;i <tamanoMatriz;i++){
        visitados[i] = false;
    }
    vector<int> distanciaMasCorta(tamanoMatriz);
    for(int i = 0;i <tamanoMatriz;i++){
        distanciaMasCorta[i] = 9999;
    }
    distanciaMasCorta[0] = 0;

    NodoArbol* raiz = new NodoArbol(nodosUsar[0]->letra);
    vector<NodoArbol*> nodosArbol(tamanoMatriz);
    for(int i = 0;i <tamanoMatriz;i++){
        nodosArbol[i] = nullptr;
    }
    nodosArbol[0] = raiz;

    for(int i = 0;i<tamanoMatriz-1;i++){
        int nodoActual = -1;
        int distanciaCorta = -1;
        for(int j = 0;j<tamanoMatriz;j++){
            if(!visitados[j] && distanciaMasCorta[j] != -1 && (distanciaCorta == -1 || distanciaMasCorta[j] < distanciaCorta)){
                nodoActual = j;
                distanciaCorta = distanciaMasCorta[j];
            }
        }
    
        if(nodoActual == -1){
            break;
        }
        visitados[nodoActual] = true;

        for(int nodoAdyacente = 0;nodoAdyacente <tamanoMatriz;nodoAdyacente++){
            if(matriz[nodoActual][nodoAdyacente] > 0 && !visitados[nodoAdyacente]){
                int nuevaDistancia = distanciaMasCorta[nodoActual]+matriz[nodoActual][nodoAdyacente];
                if(nuevaDistancia < distanciaMasCorta[nodoAdyacente]){
                    distanciaMasCorta[nodoAdyacente] = nuevaDistancia;

                    if(!nodosArbol[nodoAdyacente]){
                        nodosArbol[nodoAdyacente] = new NodoArbol(nodosUsar[nodoAdyacente]->letra);
                    }
                    nodosArbol[nodoAdyacente]->raiz = nodosArbol[nodoActual];
                }
            }
        }
    }
    int indiceDestino = - 1;
    for(int k = 0;k<nodosUsar.size();k++){
        if(nodosUsar[k] == nodoAEncontrar){
            indiceDestino = k;
            break;
        }
    }
    cout<<"Distancia mas corta al Nodo: "<<nodoAEncontrar->letra<<" es "<<distanciaMasCorta[indiceDestino]<<endl;
    return nodosArbol[indiceDestino];
}//------------------------------------------------------------------------------------------


//imprimir las letras del camino mas corto
void imprimirCaminoMasCorto(NodoArbol* nodoAEncontrar){
    vector<char> camino;
    NodoArbol* nodoActual = nodoAEncontrar;

    while(nodoActual != nullptr){
        camino.push_back(nodoActual->letra);
        nodoActual = nodoActual->raiz;
    }
    cout<<"El camino mas corto al nodo es: ";
    for(int i = camino.size()-1;i>=0;i--){
        cout<<camino[i];
        if(i>0){
            cout<<" ";
        }
    }
    cout<<endl;

}//----------------------------------------


//MAIN=======
int main(){
    int tamanoMatriz = 0;
    vector<vector<int>> matriz;

    if(cargarArchivoMatriz(matriz,tamanoMatriz,"Matriz4.txt")){
        //imprimirMatriz(matriz,tamanoMatriz);
        vector<Nodo*> nodosUsar = letrasNodos(tamanoMatriz);
        NodoArbol* nodoCaminoCorto = dijkstra(matriz,tamanoMatriz,nodosUsar);
        imprimirCaminoMasCorto(nodoCaminoCorto);
    }else{
        cout<<"Error al cargar la Matriz"<<endl;
    }
    return 0;
}//==========