/* 
 * File:   Maquina.h
 * Author: Drebin
 *
 * Created on 9 de noviembre de 2015, 17:25
 */

#ifndef MAQUINA_H
#define	MAQUINA_H
#include "Nodo.h"
#include "Grafo.h"
#include <vector>
#include <algorithm>
#include <queue>
class Maquina {
private:
    Nodo *primerTrabajo;
    bool guardarPrimerTrabajo;
    int numeroMaquinas;   
    vector<vector<Nodo*> > matrizMaquinas; //Maquina/Trabajo
    vector<int> tiempoMaquinas;
    
    //Historial del orden en el que se han ido introduciendo las operaciones
    //en las maquinas
    queue<Nodo*> colaOperaciones;
   
    Nodo* nodoDependiente(Nodo *operacion);
    
public:
    Maquina(int numMaquinas);    
    Maquina();
    virtual ~Maquina();
    Nodo* getPrimerTrabajo();
    
   
    void insertarEnMaquina(Nodo *operacion);
    void mostrarEstadoActual();
    void limpiar();
    int getCMAX();
    int getTamMatriz();   
    vector<vector<Nodo*> > getMatrizMaquinas() const;
    int getNumMaquinas();
    int getNumTrabajos();    
    void guardarEnMaquina(Nodo *operacion);
    void inicializarMaquina(int numMaquinas);
    void cambiarTrabajo(Nodo *operacionA,Nodo *operacionB);
    void cambiarTrabajoPOS(int maquina, int trabajoA, int trabajoB);
    int getPosicion(Nodo *operacion);
    void restablecerOperaciones();
      
    void copiarMaquina(Maquina &maquina);
    queue<Nodo*> getColaOperaciones() const;
    void restablecerMaquinaCola(queue<Nodo*> cola);

};




#endif	/* MAQUINA_H */

