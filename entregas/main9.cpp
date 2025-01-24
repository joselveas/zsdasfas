//Guillermo Arturo Bustamante Rodriguez
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
class Nodo{
    public:
        char letra;
        int distancia;
        vector<Nodo*> Phijos; //vector de posibles nodos hijos 
    Nodo(char _letra, int _distancia){
        letra=_letra;
        distancia=_distancia;

    }
};
vector<vector<int>> leerTxT(string Archivo, int& cantidadnodos){
    ifstream file(Archivo);
    file>>cantidadnodos;

    vector<vector<int>>matriz(cantidadnodos,vector<int>(cantidadnodos));//se crea la matriz (grafo) que vamos a retornar
    for(int i=0;i<cantidadnodos;i++){//se usa un doble for para la lectura de la matriz de adyacencia
        for(int j=0;j<cantidadnodos;j++){
            file>>matriz[i][j];
            if(file.peek()==','){// condicion para ignorar la "coma"
                file.ignore();
            }
        }
    }
    
    return matriz;






}
Nodo* creararbol(int inicio,vector<char>&nodos,vector<int>&padres, vector<int>&distancias){
    int nodosP=padres.size();
    vector<Nodo*> hijos(nodosP,nullptr);// se crea un vector para agregar los hijos posibles 
    for(int i=0;i<nodosP;i++){
        hijos[i]=new Nodo(nodos[i],distancias[i]);
    }
    Nodo* raiz=hijos[inicio];
    for(int j=0;j<nodosP;j++){ //agregar al vector los hijos 
        if(padres[j]!=-1 && j!=inicio ){
            hijos[padres[j]]->Phijos.push_back(hijos[j]);
        }
    }
    
    return raiz; // retorna el arbol


}
void recorrercamino(Nodo* arbol, bool& encontrado, vector<char>&camino, char destino){
    if(arbol==nullptr || encontrado){//si no lo ha encontrado, o el arbol es nulo, retorna.
        return;
    }
    camino.push_back(arbol->letra);//agrega al vector camino la letra del nodo
    if(arbol->letra == destino){//si llega al nodo que buscaba, la variable "encontrado" le asigna true.
        encontrado=true;
        return;
    }
    for(Nodo* hijo:arbol->Phijos){// recorre el arbol de manera recursiva hasta llegar al destino
        recorrercamino(hijo,encontrado,camino,destino);
        if(encontrado){
            return;
        }
    }
    camino.pop_back(); // elimina para limpiar el vector
}
void Algoritmodijkstra(vector<vector<int>>& grafo, int inicio,int destino,vector<char>&nodos){
    int n=grafo.size();
    vector<int> distanciasNodos(n,9999);
    vector<bool> visitados(n,false);
    vector<int> padres(n,-1);
    vector<char>camino;
    distanciasNodos[inicio]=0;
    bool encontrado=false;

    for(int i=0; i<n-1;i++){
        int NOvisitado=-1;//indice para verificar los nodos aun no visitados que no se puedan acceder
        int distanciaMin=9999;
        for(int j=0;j<n;j++){
            if(!visitados[j] && distanciasNodos[j]<distanciaMin){//si no esta visitado y es una distancia menor, es true
                distanciaMin=distanciasNodos[j];//ingreso las menores distancias a la variable "distanciaMin" 
                NOvisitado=j;
            }
        }


        if(NOvisitado==-1){
            break;// nodos que no se pueden acceder
        }
        visitados[NOvisitado]=true;

        for(int z=0;z<n;z++){// guarda la distancia mas corta 
            if(!visitados[z] && grafo[NOvisitado][z] && distanciasNodos[NOvisitado]!=9999 && grafo[NOvisitado][z]+distanciasNodos[NOvisitado]<distanciasNodos[z]){
                distanciasNodos[z]=grafo[NOvisitado][z]+distanciasNodos[NOvisitado];
                padres[z]=NOvisitado;
            }
        }
    }
    
    cout<<"distancia mas corta: "<<distanciasNodos[destino]<<endl;

    Nodo* arbol=creararbol(inicio,nodos,padres,distanciasNodos);
    recorrercamino(arbol,encontrado,camino,nodos[destino]);
    for(int i=0;i<camino.size();i++){// se imprime por pantalla el camino mas corto
        cout<<camino[i];
        if(i<camino.size()-1){
            cout<<"->";
        }
    }
    cout<<endl;

}
int main(){
    int cantidadnodos;
    vector<vector<int>> grafo=leerTxT("matriz.txt",cantidadnodos);//se crea el grafo

    vector<char> nodos(cantidadnodos);
    
    cout<<"Nodos disponibles: "<<endl;
    for(int i=0;i<cantidadnodos;i++){
        nodos[i]='A'+i; //asigna cada caracter
        
        cout<<nodos[i];// imprime los disponibles
        if(i<cantidadnodos-1){
            cout<<" , ";
        }
    }
    cout<<endl;
    char destino;
    int indiceDestino;
    cout<<"Ingrese el nodo destino: "<<endl;
    cin>>destino;
    
    while(true){
        indiceDestino=destino-'A';//se le asigna el "inidice del nodo"
    
        
        if(indiceDestino<0 || indiceDestino>=cantidadnodos){
            cout<<"nodo ingresado incorrectamente "<<endl;
            cout<<"Ingrese el nodo destino nuevamente: "<<endl;
            cin>>destino;

        }else{
            break;
        }
            
    }
    
    Algoritmodijkstra(grafo,0,indiceDestino,nodos);



    return 0;
}