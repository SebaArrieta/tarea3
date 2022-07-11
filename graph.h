#include <iostream>
#include <fstream>
#include <climits>

using namespace std;
#define VISITADO 1
typedef int tVertice;
typedef int tipoClave;
const int INFINITO = INT_MAX;

/*****
* struct data
******
* struct que contiene la información del grafo. 
*****/
struct data{
    tVertice numVertice;
    string nombre;
    int id;
    int grado = 0;
    int marca = 0;
};

/*****
* class Graph
******
* Clase que sirve para crear un nodo mediante una matriz y una tabla hash.
* Permite añadir información a cada vertice, añadir pesos u obtenerlos, asignar una marca, obtener información de un vertice y realizar algoritmo floyd-warshall
*****/
class Graph{
    private:
        int vertices;
        int **matrix; //matriz que contendra pesos 
        data *info; // se crea tabla hash para almacenar datos asociados a cada vertice
        
    public:
        Graph(int n);
        ~Graph();
        void setVertices(int num, int id);
        void setVertices(int num, string name);
        void setEdges(int i, int j, int weight);
        data getData(int num);
        int nVertex();
        void setMark(tVertice v, int marca);
        int weight(tVertice v1, tVertice v2);
        void floydsWarshalls(int** matriz);


};

/*****
* Graph::Graph
******
* Constructor de la clase que inicializa la matriz y la tabla hash
******
* Input:
* int n : Numero de vectores del cual se creará el grafo
******
* Returns:
* No tiene
*****/
Graph::Graph(int n){
    vertices = n; //se establece el numero de vertices

    // se crea la matriz adyacente de tamaño [n][n]
    matrix = new int*[n]; 
    for(int i = 0; i<n;i++){ 
        matrix[i] = new int[n];
    }

    //tabla hash de tamaño n con la informacion de cada vertice
    info = new data[vertices]; 

}

/*****
* Graph::~Graph
******
* Destructor encargado de eliminar datos pedidos mediante memoria dinamica.
******
* Input:
* No tiene.
******
* Returns:
* No tiene.
*****/
Graph::~Graph(){
    // se elimina la matriz adyacente de tamaño [n][n]
    for(int i = 0; i<vertices;i++){ 
        delete[] matrix[i];
    }

    //se elimina ultimo dato de la matriz y el arreglo info
    delete[] matrix;
    delete[] info;
}

/*****
* void Graph::setVertices
******
* Metodo encargado de establecer id y numero de vertice de los grafos
******
* Input:
* tVertice num : Numero del vertice a establecer
* int id: ID del vertice a establecer
******
* Returns:
* void
*****/
void Graph::setVertices(tVertice num, int id){
    info[num].id = id; //guarda id del vertice correspondiente
    info[num].numVertice = num; //guarda num del vertice correspondiente
}

/*****
* void Graph::setVertices
******
* Metodo encargado de establecer el nombre de los vertices
******
* Input:
* tVertice num: Numero del vertice
* string name: Nombre del vertice a establecer
******
* Returns:
* void
*****/
void Graph::setVertices(tVertice num, string name){ info[num].nombre = name; }

/*****
* void Graph::setEdges
******
* Metodo encargado de establecer el peso entre par de vertices y de contar los grados de cada vertices
******
* Input:
* tVertice v1 : Numero del primer vertice 
* tVertice v2 : Numero del segundo vertice
* int weight : Peso que se establecerá entre v1 y v2
******
* Returns:
* void
*****/
void Graph::setEdges(tVertice v1, tVertice v2, int weight){
    matrix[v1][v2] = weight; // agrega el peso 
    if(weight != 0){ info[v2].grado++; } //si el peso es distinto de 0 existe un arco, por lo que el grado aumenta
}

/*****
* int Graph::nVertex
******
* Metodo encargado de retornar la cantidad de vertices del grafo
******
* Input:
* No tiene
******
* Returns:
* int : Retorna la cantidad de vertices del grafo
*****/
int Graph::nVertex(){ return vertices; }

/*****
* data Graph::getData
******
* Metodo que obtiene la información de un vertice
******
* Input:
* tipoParámetro NombreParámetro : Descripción Parámetro
* tVertice num : Numero del vertice a retornar la información
******
* Returns:
* data : Retorna la información del vertice pasado por parametro
*****/
data Graph::getData(tVertice num){ return info[num]; }

/*****
* void Graph::setMark
******
* Metodo encargado de establecer la marca de un vertice
******
* Input:
* tVertice v : Numero de vertice al que se le quiere establecer la marca
* int marca : Marca a establecer
******
* Returns:
* void
*****/
void Graph::setMark(tVertice v, int marca){ info[v].marca = marca; }

/*****
* int Graph::weight
******
* Metodo encargado de obtener el peso de un par de vertices
******
* Input:
* tVertice v1 : Numero del primer vertice
* tVertice v2 : Numero del segundo vertice
******
* Returns:
* int : Retorna el peso que existe entre los vertices v1 y v2
*****/
int Graph::weight(tVertice v1, tVertice v2){ return matrix[v1][v2]; }

/*****
* void Graph::floydsWarshalls
******
* Metodo basado en el algoritmo Floyd-Warshall.
* El cual se encarga de comparar los caminos entre cada par de vertice para obtener mediante una matriz, el camino de menor costo entre cada par de vertice
******
* Input:
* int** matriz : puntero doble que corresponde a una matriz al cual se le estableceran los menores costos entre cada par de vertices
******
* Returns:
* void
*****/
void Graph::floydsWarshalls(int** matriz){
    int size = nVertex();
    int i,j,k;

    //Mediante un doble for se recorre cada peso del grafo para establecer a los vecinos inalcanzables como infinito
    for(i = 0; i < size; i++){
        for(j = 0; j<size; j++){
            if(matrix[i][j]!=0){ //si existe un arco
                matriz[i][j] = matrix[i][j];
            }else if(i == j){ 
                matriz[i][j] = 0;
            }else{
                matriz[i][j] = INFINITO; //vecino inalcanzable al inicio vale infinito
            }
        }
    }

    //Mediante un triple for se va comparando cada camino entre par de vertices, viendo si existe una mejora, hasta encontrar el optimo
    for(k = 0; k < size; k++){
        for(i = 0; i < size; i++){
            for(j=0 ; j < size; j++){
                if(matriz[i][j] > (matriz[i][k] + matriz[k][j]) && matriz[i][k] != INFINITO && matriz[k][j] != INFINITO){
                    matriz[i][j] = (matriz[i][k] + matriz[k][j]);
                }
            }
        }
    }
}   


