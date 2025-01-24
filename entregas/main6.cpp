//Nombre: Ignacio Alejandro García Castillo

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <fstream>

using namespace std;

//clase Nodo para el Shortest path tree
class Nodo{

    public:
    pair<char,int> ci;
    vector<Nodo*> hijos;
    Nodo(char caracter, int peso){
        this->ci.first=caracter;
        this->ci.second=peso;
    }

};

//Funcion para añadir nodos a un nodo dentro de un arbol
void anadirNodo(Nodo* raiz, Nodo* anadir){

    if(raiz==nullptr || anadir == nullptr){return;}
    raiz->hijos.push_back(anadir);

}

/*
Busca un nodo dentro del arbol Shortest path tree con las caracteristicas otorgadas, utilizado
principalmente dentro del dijkstra para buscar el nodo "perteneciete" al siguiente indice a 
explorar dentro del grafo y poder asignar los nodos vecinos
*/
Nodo* buscarNodo(Nodo* raiz,char caracter, int camino){

    if(raiz==nullptr){return nullptr;}
    if(raiz->ci.first==caracter && raiz->ci.second==camino){return raiz;}

    Nodo* temp;

    for(int i = 0; i<raiz->hijos.size();i++){

        temp = buscarNodo(raiz->hijos[i],caracter,camino);
        if(temp!=nullptr){return temp;} 

    }

    return nullptr;

}


/*
dijkstra: modifica el vector "menorCamino", y genera el Shortest Path Tree, camino más corto.
En este caso el algoritmo recorre un nodo, calcula los caminos de sus vecinos e itera sobre el
"Nodo"(indice) no visitiado con el camino más corto. Existian otros metodos con "cola de prioridad", sin embargo,
el metodo que vimos en clase no se parecia a ese algoritmo, el algoritmo que estoy implementando
me parece mas adecuado y aplica lo visto durante el curso.
*/
void dijkstra(vector<int>& menorCamino, vector<vector<int>>& matAdy,Nodo* raiz){

    queue<int> cola;
    queue<Nodo*> colaNodos;
    colaNodos.push(raiz);
    vector<int> visitados(matAdy.size(), false);

    cola.push(0);
    menorCamino[0] = 0;

    while(!cola.empty()){

        int actual = cola.front();
        cola.pop();
        Nodo* actualNodo = colaNodos.front();
        colaNodos.pop();
        visitados[actual] = true;
        int menor = 2147483647;
        int indiceMenor = -1;

        for(int ady = 0; ady<matAdy.size();ady++){

            if(matAdy[actual][ady] != 0){

                int suma = matAdy[actual][ady] + menorCamino[actual];
                char caracter = ady+65;
                if(actual!=ady){anadirNodo(actualNodo,new Nodo(ady+65,suma));}


                if(suma < menorCamino[ady]){
                    menorCamino[ady]=suma;
                    
                }

            }
   
        }
        
        for(int i = 0;i<menorCamino.size();i++){
            
            if(menorCamino[i] < menor && visitados[i] == false){
                menor = menorCamino[i];
                indiceMenor = i;
            }

        }

        if(indiceMenor != -1) {
            cola.push(indiceMenor);
            colaNodos.push(buscarNodo(raiz,indiceMenor+65,menorCamino[indiceMenor]));

        }

    }
        
}

/*
Función recursiva, su principal utilidad es rellenar el vector "camino" con el camino más
corto hasta el nodo objetivo, todo esto atravez de un recorrido, dfs, dentro del arbol/grafo
*/
bool caminoMasCortoA(Nodo* raiz,vector<char>& camino, char objetivo, int objetivoInt, int iteracion){

    if(raiz  == nullptr){return false;}

    camino[iteracion] = raiz->ci.first;

    if(raiz->ci.first == objetivo && raiz->ci.second == objetivoInt){return true;}

    for(int i = 0;i<raiz->hijos.size();i++){

        if(caminoMasCortoA(raiz->hijos[i],camino,objetivo,objetivoInt,iteracion+1)){
            return true;
        }

    }

    camino[iteracion] = '@';
    return false;    
}

//Metodo para limpiar la memoria del programa
void liberarMemoria(Nodo* raiz){

    if(raiz == nullptr){return;}

    for(Nodo* hijo:raiz->hijos){
        liberarMemoria(hijo);
    }

    delete raiz;

}

int main(){

    string nombreTXT;

    cout<<"Ingrese el nombre de el Archivo para leer la matriz"<<endl;

    getline(cin,nombreTXT);

    ifstream archivo(nombreTXT);
    string opcion = "2";

    while(!archivo){


        if(opcion == "2") {cout<<"El archivo no existe"<<endl;}
        cout<<"1.- Salir\n2.-Volver a intentar"<<endl;
        
    
        getline(cin,opcion);

        if(opcion=="1"){

            archivo.close();
            return 0;

        } else if(opcion == "2"){

            cout<<"Ingrese el nombre de el Archivo para leer la matriz"<<endl;
            getline(cin,nombreTXT);
            archivo.close();
            archivo.open(nombreTXT);

        } else {
            
            cout<<"Opcion incorrecta"<<endl;

        }

        

    }

    string linea;

    int n;
    
    archivo>>n;

    if(n <= 0){
        cout<<"No hay una matriz dentro del Archivo."<<endl;
        archivo.close();
        return 0;
    }

    if(n > 26){
        cout<<"La matriz excede el limite esperado(26 X 26, A...Z)."<<endl;
        archivo.close();
        return 0;
    }

    getline(archivo,linea);

    vector<vector<int>> matAdy(n, vector<int>(n, 0));

    int x = 0;
    int y = 0;

    int i = 0;

    while(getline(archivo,linea)){

        string numero = "";
        x=0;
        i=0;
        while(i!=linea.size()){

            while(i<linea.size() && linea[i]!=','){

            numero += linea[i];
            i++;

            }

        if(i<linea.size() && linea[i]==','){i++;}
        int numeroM = stoi(numero);
        numero = "";
        matAdy[y][x] = numeroM;
        x++;
        }
        y++;
    }

    archivo.close();

    vector<int> menorCamino(matAdy.size(), 2147483647);
    
    Nodo* raiz = new Nodo(65,0);


    dijkstra(menorCamino, matAdy, raiz);

    cout<<"¿Donde desea ir?"<<endl;


    for(int k = 0; k<menorCamino.size();k++){

    
        char caracter = k+65;

        cout<<caracter;
        if(k!=menorCamino.size()-1){cout<<",";}

    }

    cout<<endl;

    string objetivo;


    getline(cin,objetivo);

    while(objetivo.size() >= 2 || objetivo[0]<=64 || objetivo[0] >= 91 || objetivo[0] > matAdy.size() + 64){

        cout<<"Opción invalida, porfavor ingrese el Nodo objetivo dentro del rango otrogado con el formato, 'A' , 'B', 'C' , etc."<<endl;

        getline(cin,objetivo);

    }

    vector<char> camino(matAdy.size(), '@');


    int objetivoInt = menorCamino[objetivo[0] - 65];



    if(buscarNodo(raiz,objetivo[0],objetivoInt) == nullptr){
        cout<<"No existe un camino hacia "<<objetivo[0]<<endl;
        return 0;
    } else {


        cout<<objetivoInt<<endl;
        caminoMasCortoA(raiz,camino,objetivo[0],objetivoInt,0);

        for(int i = 0; i<camino.size();i++){

            cout<<camino[i];
            if(camino[i]!=objetivo[0]){
                cout<<"->";
            } else {i = camino.size();}

        }

    }

    liberarMemoria(raiz);
    cout<<endl;
    
    return 0;

}