/* 
 * File:   AGGE.h
 * Author: Drebin
 *
 * Created on 12 de noviembre de 2015, 18:34
 */

#ifndef AGGE_H
#define	AGGE_H
#include "Grafo.h"
#include "Maquina.h"
#include <vector>
#include <cmath>
#include <stdio.h>
using namespace std;

class AGGE {
private:
    
    Maquina mejorMaquina;
   
    vector<Maquina*> padres;
    vector<Maquina> hijos;
    int tamPoblacion;
    int numMaquinas;
    int numTrabajos;
       
    //Numero de cromosomas que se van a cruzar
    int numCromosomasCruce;
   
    int buscarMejorMaquina(Maquina *poblacion[]);
    void SXX(Nodo *padre1[],Nodo *padre2[],int numOperaciones,int indicePadres);
    int calcularSubSecuencia(Nodo *subSecuencia[],Nodo *padre[],int tamSubSecuencia,int numOperaciones);
    void seleccion(Maquina *poblacion[]); 
    void cruce();
    void mutacion();
    void reparacion(int indiceHijo,Grafo *grafo[]);
    
    //Semilla para generar los numeros aleatorios
    int rdtsc();
    
    
public:
    AGGE(int _tamPoblacion,int _numMaquinas,int _numTrabajos);    
    virtual ~AGGE();
    void aplicarAlgoritmoAGGE(Maquina *poblacion[],Grafo *grafo[]);
   
};

#endif	/* AGGE_H */

