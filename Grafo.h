/* 
 * File:   Grafo.h
 * Author: Drebin
 *
 * Created on 9 de noviembre de 2015, 9:37
 */

#ifndef GRAFO_H
#define	GRAFO_H
#include "Nodo.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <list>
using namespace std;
      

class Grafo {
private:    
    string nombreFichero;
    int numTrabajos;
    int numMaquinas;
    int tiempoOptimo;     
    vector<Nodo> vectorNodos;
    void caminoCritico(Nodo *inicial,int cMaxActual,int &cMax,
                       vector<list<Nodo*> > &recorrido,int &numCamino);
    
public:
    Grafo(const string _nombreFichero);
    Grafo();
    virtual ~Grafo();
    void SetTiempoOptimo(int tiempoOptimo);
    int GetTiempoOptimo() const;
    void SetNumMaquinas(int numMaquinas);
    int GetNumMaquinas() const;
    void SetNumTrabajos(int numTrabajos);
    int GetNumTrabajos() const;
    void SetNombreFichero(string nombreFichero);
    string GetNombreFichero() const;
    vector<Nodo>& GetVectorNodos();
    Nodo* getNodoID(int id);
   
    //-----Funciones Auxiliares------
    
    void visualizarGrafo();
    Grafo& operator=(const Grafo &orig);
    list<Nodo*> nodosCaminoCritico(Nodo *inicial);
    long int calcularCMAX(Nodo *inicial);
    void restablecerGrafo(const vector<vector<Nodo*> > &matrizMaquinas);

};

#endif	/* GRAFO_H */

