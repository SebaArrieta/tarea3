#include <iostream>
#include <cstring>
#include <string>
#include <math.h>
#include "mergeSort.h"
#include "minHeap.h"

#define M 128

using namespace std;

struct ranura{
    string termino;
    string definicion;
    int consultas;
};

ranura HT[M];

    class CacheDiccionario{
    private:
        int nElem;
        int Ctotal;
        int Cconocidos;
        int Cdesconocidos;
        int CleanCount;
        int HitsCount;
        int MissCount;
        int AcCount ;
    public:
        
/*****
* void CacheDiccionario
******
* Esta funcion inicializa el TDA estableciendo las variables iniciales en 0
******
* Input:
* 
* .......
******
* Returns:
* 
*****/

    CacheDiccionario(){
        nElem = 0;
        Ctotal = 0;
        Cconocidos = 0;
        Cdesconocidos = 0;
        CleanCount = 0;
        HitsCount = 0;
        MissCount = 0;
        AcCount = 0;
    }
        
 /*****
* int h
******
* La funcion h da la posicion inicial de algun elemento a la hora de incertarlo en el hashing, para esto divide el termino entre tres para sumar la equivalencia en ascii 
y asi retornar la suma total de sus partes.
******
* Input:
* string termino : el termino es la palabra a ingresar en el diccionario y funciona como la clave del elemento para el hashing.
* .......
******
* Returns:
* int, la suma de las distintas secciones en la que se dividio la palabra seleccionada.
*****/

    int h(string termino){
        int contador= 0;
        string word = "";
        for(int i = 0; i < (termino.length() - (termino.length()%3)); i++){

            int wordInt = termino[i];
            for(int j = 0; j < to_string(wordInt).length(); j++){
                word+= to_string(wordInt)[j];
            }
            
            if((i+1)%3 == 0){
                contador += stoi(word);
                word = "";
            }

        }

        word = "";
        int res = (termino.length() + (termino.length()%3)) - termino.length();
        for(int i = termino.length() - res; i < termino.length(); i++){
            int wordInt = termino[i];
            for(int j = 0; j < to_string(wordInt).length(); j++){
                word+= to_string(wordInt)[j];
            }
        }

        if(word.length() > 0)
            contador += stoi(word);
        
        return contador % M;
    }
   
 /*****
* unsigned long p
******
* la funcion p resuelve las colisiones que podrian ocurrir durante la insercion de los elementos al hashing, utilizando .
******
* Input:
* string termino : termino seleccionado que colisiona en la insercion del hashing.
  int i: hace referencia a la cantidad de colisiones que han ocurrido luego de implementar la funcion varias veces.
* .......
******
* Returns:
* int, retorna la cantidad de espacios que debe avanzar para establecer la nueva posicion del elemento.
*****/

    unsigned long p(string termino, int i){
        return i*i*2 + 5*i + 1;
    }
  
  /*****
* bool query
******
* esta funcion se encarga de consultar los elementos en el diccionario y tambien para comprobar su existencia.
******
* Input:
* string termino : termino seleccionado a buscar en el diccionario.
  string &significado: string por donde pasara la definicion del termino en caso de encontrarse.
* .......
******
* Returns:
* bool, retorna true si se encuentra el temrino en el diccionario en caso contrario se retorna un false.
*****/

    bool query(string termino, string &significado){
        Ctotal += 1;
        int inicio;
        int pos = inicio = h(termino);
        for(int i = 1; !HT[pos].termino.empty() && HT[pos].termino != termino; i++){
            pos = (inicio + p(termino, i)) % M;
            AcCount += 1 ;
        }
        
        if(HT[pos].termino == termino){
            significado = HT[pos].definicion;
            HT[pos].consultas += 1;
            if(HT[pos].definicion.length() > 0){
                Cconocidos += 1;
                HitsCount += 1;
            }else{
                Cdesconocidos -=1;
                HitsCount += 1 ;
            }
            return 1;
        }else{
            significado = "";
            return 0;
            MissCount += 1 ;
        }
    };
        
/*****
* void clean
******
* esta funcion se encarga de borrar la mitad de los elementos del hashing cuando esta se encuentra llena borrando los que tengan menos consultas registradas.
******
* Input:
* 
* .......
******
* Returns:
* 
*****/   

    void clean(){
        CleanCount += 1;
        tColaP list;

        for(int i = 0; i<M;i++){
            nodo New;
            New.consultas = HT[i].consultas;
            New.key = i;
            
            list.insertColaP(New);
        }

        for(int i = 0; i<M/2;i++){
            HT[list.findMin()].termino = "";
            HT[list.findMin()].definicion = "";
            HT[list.findMin()].consultas = 0;
            list.removeMin();
            nElem-=1;
        }

    }
        
/*****
* bool query
******
* esta funcion se encarga de insertar los elementos en el hashing.
******
* Input:
* string termino : termino seleccionado a insertar en el diccionario.
  string significado: significado vinculado al termino.
* .......
******
* Returns:
* 
*****/

    void insert(string termino, string significado){
        if(nElem == M){
            clean();
        }
        int inicio;
        int pos = inicio = h(termino);
        for(int i = 1; !HT[pos].termino.empty() && HT[pos].termino != termino; i++){
            pos = (inicio + p(termino, i)) % M;
        }

        if(HT[pos].termino == termino){
            HT[pos].definicion = significado;
        }else{
            HT[pos].termino = termino;
            HT[pos].definicion = significado;
            nElem+=1;
        }

    }
        
    /*****
* bool querystats
******
* esta funcion retorna las estadisticas vinculadas a la funcion query como las consultas totales, las consultas con significado conocido y desconocidos.
******
* Input:
* int &total : variable en donde se indicara el total de consultas realizadas hasta el momento.
  int &conocidos: consultas realizadas a terminos con significados no vacios.
  int &desconocidos: consultas realizadas a terminos con significados vacios.
* .......
******
* Returns:
* bool, retorna true si se encuentra el temrino en el diccionario en caso contrario se retorna un false.
*****/

    void querystats(int &total, int &conocidos, int &desconocidos){
        total = Ctotal;
        conocidos = Cconocidos;
        desconocidos = Cdesconocidos;
    }
        
 /*****
* bool querystats
******
* esta funcion retorna las estadisticas vinculadas a al rendimiento del TDA como la cantidad de veces que se borraron elementos para ingresar unos nuevos o
  cuantas veces las consultas realizadas en el query estuvieron relacionadas a terminos existentes.
******
* Input:
* int& accesses: cantidad de accesos a las ranuras del hashing
  int& hits: cantidad de consultas a terminos existentes.
  int& misses: cantidad de consultas a terminos no existentes.
  int& cleanups: cantidad de veces que se borraron elementos de la tabla hashing.
* .......
******
* Returns:
* bool, retorna true si se encuentra el temrino en el diccionario en caso contrario se retorna un false.
*****/

    void perfstats(int& accesses, int& hits, int& misses, int& cleanups){
        accesses = AcCount;
        hits = HitsCount;
        misses = MissCount; 
        cleanups = CleanCount;
        
    }
};
