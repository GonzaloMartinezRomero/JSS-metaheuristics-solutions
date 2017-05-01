/* 
 * File:   Nodo.cpp
 * Author: Drebin
 * 
 * Created on 29 de septiembre de 2015, 11:09
 */

#include "Nodo.h"


Nodo::Nodo(int _tiempo,int _numTrabajo,int _numMaquina) {
    this->tiempo=_tiempo;
    this->numTrabajo=_numTrabajo;
    this->numMaquina=_numMaquina;
    this->final=false;
    this->ID=0;
    this->siguiente=0;
    this->tiempoMaxMaq=0;
    this->tiempoMinMaq=0;
    this->sucesor=0;
    this->seleccionado=false;
    
}
Nodo::Nodo(){
    this->tiempo=-1;
    this->numTrabajo=-1;
    this->numMaquina=-1;
    this->final=false;
     this->ID=0;
    this->siguiente=0;
    this->tiempoMaxMaq=0;
    this->tiempoMinMaq=0;
    this->sucesor=0;
    this->seleccionado=false;
   
}

//Nodo::Nodo(const Nodo& orig){
//    
//    this->sucesores.clear();
//    this->ID=orig.ID;
//    this->seleccionado=orig.seleccionado;
//    this->final=orig.final;
//    this->numMaquina=orig.numMaquina;
//    this->numTrabajo=orig.numTrabajo;
//    this->siguiente=0;
//    this->sucesor=0;
//    this->tiempo=orig.tiempo;
//    this->tiempoMaxMaq=orig.tiempoMaxMaq;
//    this->tiempoMinMaq=orig.tiempoMinMaq;
//   
//    
//}

Nodo::~Nodo() {
}

void Nodo::anadirSucesor(Nodo* _nodo){
    sucesores.push_back(_nodo);        
}

void Nodo::vaciarSucesores( ){
    sucesores.clear();
}


void Nodo::setNumMaquina(int numMaquina) {
    this->numMaquina = numMaquina;
}

int Nodo::getNumMaquina() const {
    return numMaquina;
}

void Nodo::setNumTrabajo(int numTrabajo) {
    this->numTrabajo = numTrabajo;
}

int Nodo::getNumTrabajo() const {
    return numTrabajo;
}

void Nodo::setTiempo(int tiempo) {
    this->tiempo = tiempo;
}

int Nodo::getTiempo() const {
    return tiempo;
}

vector<Nodo*> Nodo::getSucesores()const{
    return sucesores;
}

void Nodo::setFinal(bool final) {
    this->final = final;
}

bool Nodo::isFinal() const {
    return final;
}


void Nodo::setSiguiente(Nodo* siguiente) {
    this->siguiente = siguiente;
}

Nodo* Nodo::getSiguiente() const {
    return siguiente;
}
 

void Nodo::setSucesor(Nodo* sucesor) {
    this->sucesor = sucesor;
}

Nodo* Nodo::getSucesor() const {
    return sucesor;
}

void Nodo::setTiempoMinMaq(int tiempoMinMaq) {
    this->tiempoMinMaq = tiempoMinMaq;
}

int Nodo::getTiempoMinMaq() const {
    return tiempoMinMaq;
}

void Nodo::setTiempoMaxMaq(int tiempoMaxMaq) {
    this->tiempoMaxMaq = tiempoMaxMaq;
}

int Nodo::getTiempoMaxMaq() const {
    return tiempoMaxMaq;
}

void Nodo::setID(unsigned ID) {
    this->ID = ID;
}

unsigned Nodo::getID() const {
    return ID;
}

void Nodo::setSeleccionado(bool seleccionado) {
    this->seleccionado = seleccionado;
}

bool Nodo::isSeleccionado() const {
    return seleccionado;
}


bool operator<(const Nodo &nodo1,const Nodo &nodo2){
    if(nodo1.getTiempo()>nodo2.getTiempo())
        return true;
    else
        return false;
}