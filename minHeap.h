#include <iostream>
#include <cstring>
#include <string>
#include <math.h>

using namespace std;

struct nodo{
    int consultas;
    int key;
};

class tColaP{
    private:
        nodo root[128];
        int size;

    public:
    tColaP(){
        size = 0;
    }

    void view(){
        for(int i = 0; i < size-1; i++){
            cout<<i+1<<". "<<root[i].consultas<<endl;
        }
    }
    
/*****
* void swap
******
* esta funcion intercambia elementos en el minheap cuando es necesario durante la insercion o el borrado de algun componente.
******
* Input:
* int pos1: poscion del primer elemento a intercambiar.
  int pos2:  poscion del segundo elemento a intercambiar.
* .......
******
* Returns:
*****/

    void swap(int pos1, int pos2){
        nodo aux = root[pos1];
        root[pos1] = root[pos2];
        root[pos2] = aux;
    }
  /*****
* void insertColaP
******
* Esta funcion inserta los elementos al minheap y asi buscar la minima cantidad de consultas por elemento cuando es necesario.
******
* Input:
* nodo n: termino a insertar al minheap acorde a la cantidad de busquedas.
* .......
******
* Returns:

*****/

    void insertColaP(nodo n){
        size+=1;
        root[size-1] = n;
        int pos = size;
        while(root[pos-1].consultas < root[(pos/2)-1].consultas){
            swap(pos-1, (pos/2)-1);

            if(pos/2 != 1){
                pos = pos/2;
            }   
        }
    }
    
  /*****
* int findMin
******
* Esta funcion entrega la posicion en el hashing del elemento con menos cantidad de busquedas.
******
* Input:
* 
* .......
******
* Returns:
*
*****/

    int findMin(){
        return root[0].key;
    }
   
 /*****
* void hundir
******
* Esta funcion recursiva hunde los elementos cuando ocurre el intercambio durante el proceso de eliminación para mantener el termino con la minima cantidad de consultas en 
la posicion 1 del minheap
******
* Input:
* int pos: inicialmente es 1 y luego va cambiando dependiendo de la nueva posicion establecida para el elemento.
* .......
******
* Returns:
*
*****/

    void hundir(int pos){
        int izq = -1;
        int der = -1;

        if(pos*2 <= size){
            izq = root[pos*2-1].consultas;
        }

        if(pos*2 + 1 <= size){
            der = root[pos*2].consultas;
        }

        if(izq == -1 && der == -1){
            return;
        }else if(izq == -1){
            izq = der + 1;
        }else if(der == -1){
            der = izq + 1;
        }

        if(der < izq){
            if(root[pos*2].consultas < root[pos-1].consultas)
                swap(pos*2, pos-1);
                hundir(pos*2+1);
        }else{
            if(root[pos*2-1].consultas < root[pos-1].consultas)
                swap(pos*2-1, pos-1);
                hundir(pos*2);
        }
    }
    
    /*****
* void removeMin
******
* Esta funcion elimina el termino con la minima cantidad de consultas y lo intercambia con siguiente elemento en la lista de minimos necesarios para la limpieza del hashing.
******
* Input:
* 
* .......
******
* Returns:
*
*****/

    void removeMin(){
        swap(0, size-1);
        size -= 1;

        hundir(1);

    }

};
