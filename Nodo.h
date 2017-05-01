/* 
 * File:   Nodo.h
 * Author: Drebin
 *
 * Created on 29 de septiembre de 2015, 11:09
 */

#ifndef NODO_H
#define	NODO_H
#include<vector>
#include <iostream>
using namespace std;


class Nodo {
    
    private:
        //Solo se usa para el nodo inicial, ya que es el unico que tiene 
        //multiples sucesores
        vector<Nodo*> sucesores;
        Nodo *sucesor;//Siguiente nodo dentro de la misma maquina
        Nodo *siguiente;//No se puede cambiar
        int tiempo;
        int numTrabajo;
        int numMaquina;
        bool seleccionado;//Sirve para el GT
        bool final;       
        unsigned ID;
        int tiempoMaxMaq;//Tiempo maximo consumido dentro de la maquina
        int tiempoMinMaq;//Tiempo minimo consumido dentro de la maquina
    
    public:
        Nodo(int _tiempo,int _numTrabajo,int _numMaquina);
        Nodo();
//        Nodo(const Nodo &orig);
        virtual ~Nodo();
        void anadirSucesor(Nodo *_nodo);         
        void vaciarSucesores();
        void setNumMaquina(int numMaquina);
        int getNumMaquina() const;
        void setNumTrabajo(int numTrabajo);
        int getNumTrabajo() const;
        void setTiempo(int tiempo);
        int getTiempo() const;        
        vector<Nodo*> getSucesores() const;
        void setFinal(bool final);
        bool isFinal() const;
        void setSiguiente(Nodo* siguiente);
        Nodo* getSiguiente() const;//Nodo siguiente inmediato                 
        void setSucesor(Nodo* sucesor);
        Nodo* getSucesor() const;
        void setTiempoMinMaq(int tiempoMinMaq);
        int getTiempoMinMaq() const;
        void setTiempoMaxMaq(int tiempoMaxMaq);
        int getTiempoMaxMaq() const;
        void setID(unsigned ID);
        unsigned getID() const;
        void setSeleccionado(bool seleccionado);
        bool isSeleccionado() const;
       
         
};
bool operator<(const Nodo &nodo1,const Nodo &nodo2);

#endif	/* NODO_H */

