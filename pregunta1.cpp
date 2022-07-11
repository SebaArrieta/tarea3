//g++ pregunta1.cpp -o hash
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

    int h(string termino){
        int contador= 0;
        string word = "";
        for(int i = 0; i < (termino.length() - (termino.length()%3)); i++){
            if((i+1)%3 == 0){
                contador += stoi(word);
                word = "";
            }

            int wordInt = termino[i];
            for(int j = 0; j < to_string(wordInt).length(); j++){
                word+= to_string(wordInt)[j];
            }

        }

        word = "";
        int res = (termino.length() + (termino.length()%3)) - termino.length();
        for(int i = termino.length() - res + 1; i < termino.length()+1 - res; i++){
            int wordInt = termino[i];
            for(int j = 0; j < to_string(wordInt).length(); j++){
                word+= to_string(wordInt)[j];
            }
            contador += stoi(word);
        }

        return contador % M;
    }

    unsigned long p(string termino, int i){
        return i*i*2 + 5*i + 1;
    }

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

    void querystats(int &total, int &conocidos, int &desconocidos){
        total = Ctotal;
        conocidos = Cconocidos;
        desconocidos = Cdesconocidos;
    }

    void perfstats(int& accesses, int& hits, int& misses, int& cleanups){
        accesses = AcCount;
        hits = HitsCount;
        misses = MissCount; 
        cleanups = CleanCount;
        
    }
};