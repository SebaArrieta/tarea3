#include "graph.h"

using namespace std;

/*****
* Graph* csvToGraph
******
* Función encargada de pasar un texto formato csv, en un grafo. 
* Recorre el texto obteniendo los id, los pesos de los arcos y los nombres de los vertices para pasarselos al grafo, el que los tomara mediante una matriz y una tabla hash.
******
* Input:
* No tiene
******
* Returns:
* Graph*, retorna un puntero de un grafo con la conversion de csv a grafo ya lista
*****/
Graph* csvToGraph(){
    ifstream csv;
    string line = "";
    string aux = "";

    csv.open("miserables.csv");
    int size = -1; //inicia en -1 para saltar el lugar [0][0] del csv que es vacio
    int cont = -1; //inicia en -1 para saltar el lugar [0][0] del csv que es vacio

    //se obtienen datos para crear el grafo
    while(getline(csv,line)){ size++; } // obtiene cantidad de vectores
    Graph* graph = new Graph(size);
    csv.clear(); //se ocupa para eliminar la flag eof
    csv.seekg(0); //el puntero ahora es el primer caracter del texto
    getline(csv,line); //obtiene primera linea del csv

    //agrega vectores correspondientes a la tabla hash
    for(unsigned int i = 0; i<line.length(); i++){ 
        if(line[i] != ';'){ // cuando encuentre un ; se detiene debido a que ya obtuvo el ID
            aux+=line[i]; //obtiene el id
        }else{
            if(cont != -1){ // se salta el [0][0] del csv, ya que no es un vertice y no tampoco un peso
                graph->setVertices(cont, stoi(aux));
            }
            aux = "";
            cont++;
        }
    }
    graph->setVertices(cont, stoi(aux)); //agrega ultimo vertice

    int cont2 = -1; // contador que va indicar la posicion [cont1][cont2]
    aux = ""; 

    //while para recorer todas las lineas del csv
    while(getline(csv,line)){
        cont2++;
        cont = -1; 
        //recorre cada caracter dentro del string, obteniendo los id
        for(unsigned int i = 0; i<line.length(); i++){
            if(line[i] != ';'){
                aux+=line[i]; //agrega peso de cada arco
            }else{
                if(cont != -1){ // se salta el 0, ya que el [0][x] no es un vertice ni un peso
                    graph->setEdges(cont, cont2, stoi(aux)); // agrega peso a la matriz
                }
                aux = "";
                cont++;
            }
        }
        graph->setEdges(cont, cont2, stoi(aux)); // agrega ultimo peso de la cola a la matriz
    }
    csv.close();

    csv.open("miserables_id_to_names.csv");
    cont = -1; //se inicia en -1 para saltar la primera linea que no contiene informacion
    getline(csv,line); //se salta la primera linea

    //while que recorre cada linea del csv
    while (getline(csv,line)){
        cont++;
        cont2 = 0;
        //for que va recorriendo cada caracter para finalmente solo obtener el nombre
        for(unsigned int i = 0; i<line.length(); i++){
            if(line[i] != ','){
                aux+=line[i];   
            }else{
                cont2++;
                if(cont2 == 2){ //si cont2 == 2 significa que saltó el id y pasó al nombre
                    graph->setVertices(cont,aux); // guarda nombre del vertice
                }
                aux = "";
            }
        }
        graph->setVertices(cont,aux); // guarda ultimo nombre del vertice
    }
    csv.close();
    return graph; //retorna un grafo creado a partir de los datos del csv
}   

/*****
* tVertice mainChar
******
* Función engargada de obtener el vertice de mayor grado dentro del grafo. El vertice corresponderá al personaje principal.
******
* Input:
* Graph* graph : Puntero de grafo que contiene toda la información del grafo a trabajar.
******
* Returns:
* tVertice, retorna numero de vertice del personaje principal (el grafo con más grado)
*****/
tVertice mainChar(Graph* graph){
    data info;
    int gradoMayor = -INFINITO;
    int numVertice;

    //Se busca el vertice de grado mayor
    for(int i = 0; i < graph->nVertex(); i++){ 
        info = graph->getData(i);
        if(info.grado > gradoMayor){
            gradoMayor = info.grado;
            numVertice = info.numVertice;
        }
    }

    return numVertice;
}

/*****
* tVertice secRelevante
******
* Función encargada de buscar el mayor camino de menor costo respecto al principal.
* Esto mediante una matriz que contiene los menores costos pasada previamente por parametro, la cual se recorrerá y buscará el mayor
******
* Input:
* Graph* graph : Puntero de grafo que contiene toda la información del grafo a trabajar.
* tVertice principal : numero del vertice del personaje principal
* int** matriz : puntero doble que corresponde a una matriz de [n][n], la cual contiene los menores costos entre cada par de vertices
******
* Returns:
* tVertice, retorna numero del personaje secundario mas relevante
*****/
int secRelevante(Graph* graph, tVertice principal, int** matriz){
    int size = graph->nVertex();
    int mayor = -INFINITO;
    int secundario;
    
    //Se busca quien tiene el mayor camino de menor costo respecto del principal
    for(int i = 0; i<size; i++){ 
        if(matriz[principal][i] > mayor){
            mayor = matriz[principal][i];
            secundario = i;
        }
    }

    return secundario;
}

/*****
* double factorial
******
* Función que mediante recursividad entre el factorial de un numero
******
* Input:
* int n : numero del factorial a calcular
******
* Returns:
* double, retorna factorial del numero entregado por parametro
*****/
double factorial(int n){
    if (n > 1)
        return factorial(n-1) * n;
    return 1;
}

/*****
* double averageDistance
******
* Funcion que calcula la longitud del camino promedio.
* Esto mediante una suma de 1 en 1 de los caminos de menores costos que existan.
* La suma se divide por la combinaciones de pares de vertices.
******
* Input:
* Graph* graph : Puntero de grafo que contiene toda la información del grafo a trabajar.
* int **matriz : puntero doble que corresponde a una matriz de [n][n], la cual contiene los menores costos entre cada par de vertices.
******
* Returns:
* double, retorna longitud del camino promedio
*****/
double averageDistance(Graph* graph, int **matriz){
    double suma = 0;
    double combinatoria;
    int size = graph->nVertex();

    //doble for que recorre la matriz de menores costos entre cada par de vertices sumando 1 si el camino existe
    for(int i = 0; i<size;i++){
        for(int j = 0; j<size;j++){
            if(matriz[i][j] != 0 && matriz[i][j] != INFINITO){
                suma++;
            }
        }
    }

    suma/=2; //divide en 2 para no considerar 2 veces los pesos de un mismo arco
    combinatoria = factorial(size)/(factorial(2)*factorial(size-2)); //combinaciones de pares de vertices
    suma = suma/combinatoria;
    return suma;
}

/*****
* double averageWeightedDistance
******
* Funcion que calcula la longitud del camino promedio con pesos.
* Esto mediante una suma de los caminos de menor costo entre cada par de vertice.
* La suma se divide por la suma de los pesos de cada arco.
******
* Input:
* Graph* graph: Puntero de grafo que contiene toda la información del grafo a trabajar.
* int **matriz : puntero doble que corresponde a una matriz de [n][n], la cual contiene los menores costos entre cada par de vertices.
******
* Returns:
* double, retorna longitud del camino promedio con pesos
*****/
double averageWeightedDistance(Graph* graph, int** matriz){
    double suma = 0;
    double pesoArcos = 0;
    int size = graph->nVertex();

    //doble for que va sumando el costo minimo entre cada par de vertices
    for(int i = 0; i<size;i++){ 
        for(int j = 0; j<size;j++){
            if(matriz[i][j] != 0 && matriz[i][j] != INFINITO){
                suma+= matriz[i][j];
            } 
        }
    }

    //doble for que suma el peso de todos los arcos
    for(int i = 0; i< size; i++){ 
        for(int j =0; j<size; j++){
            pesoArcos+=graph->weight(i,j);
        }
    }

    suma/=2; //divide en 2 para no considerar 2 veces los pesos de un mismo arco
    pesoArcos/=2; //divide en 2 para no considerar 2 veces los pesos de un mismo arco
    suma = suma/pesoArcos;
    return suma;
}

/*****
* void salida
******
* Función encargada de mostrar en pantalla: personaje principal, personaje secundario más relevante, diametro y diametro con pesos.
* Esto lo hace mediante llamadas a otras funciones para obtener los datos requeridos
******
* Input:
* No tiene
******
* Returns:
* void
*****/
void salida(){
    //se crea un grafo mediante el csv y obtiene datos
    Graph* grafo = csvToGraph();
    data info;
    int size = grafo->nVertex();

    //matriz de [n][n] que va a contener costos entre cada par de vertices
    int** costosMin = new int*[size];
    for(int i = 0; i<size;i++){
        costosMin[i] = new int[size];
    }

    //se obtiene costos minimos entre cada par de vertices
    grafo->floydsWarshalls(costosMin); 

    //se obtienen datos requeridos mediante funciones
    tVertice principal = mainChar(grafo);
    tVertice secRel = secRelevante(grafo, principal, costosMin);
    double diametro = averageDistance(grafo,costosMin);
    double diametroPeso = averageWeightedDistance(grafo,costosMin);

    //se muestran en pantalla los datos requeridos
    info = grafo->getData(principal);
    cout<<"Personaje principal: " << info.nombre<<endl;

    info = grafo->getData(secRel);
    cout<<"Personaje secundario mas relevante: " << info.nombre<<endl;

    cout<<"Diametro: " << diametro<<endl;

    cout<<"Diametro (con pesos): " << diametroPeso<<endl;

    //se elimina matriz pedida por memoria dinamica de [n][n]
    for(int i = 0; i<size;i++){ 
        delete[] costosMin[i];
    }
    delete[] costosMin;

}

/*****
* int main
******
* Función main accedida al iniciar el programa encargada de llamar a la funcion salida
******
* Input:
* No tiene
******
* Returns:
* int, int que indica que el prograba acaba, no es necesario 
*****/
int main(){
    salida();
    return 0;
}
