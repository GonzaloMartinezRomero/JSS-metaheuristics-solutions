/* 
 * File:   Maquina.cpp
 * Author: Drebin
 * 
 * Created on 9 de noviembre de 2015, 17:25
 */

#include <algorithm>

#include "Maquina.h"

Maquina::Maquina(int numMaquinas) {
    matrizMaquinas.resize(numMaquinas);
    tiempoMaquinas.resize(numMaquinas,0);
    guardarPrimerTrabajo=false;
    this->numeroMaquinas=numMaquinas;
    this->primerTrabajo=0;
   
    
}
Maquina::Maquina(){
      
}

Maquina::~Maquina() {
}

Nodo* Maquina::getPrimerTrabajo(){
    return this->primerTrabajo;
}
/*
 Devuelve el nodo anterior del que depende la operacion actual
 */
Nodo* Maquina::nodoDependiente(Nodo* operacion){
    
    for(int i=0;i<numeroMaquinas;i++){
        if(i!=operacion->getNumMaquina()-1){
            
            for(int j=0;j<matrizMaquinas[i].size();j++){
                if(matrizMaquinas[i][j]->getSiguiente()==operacion){
                    return matrizMaquinas[i][j];
                }
            }
            
        }
    }
    return 0;
}


/*
 * Cada vez que se almacena un trabajo, se calcula sus tiempos y 
 * sucesores
 */
void Maquina::insertarEnMaquina(Nodo* operacion){
    int numOperaciones=0;
    if(operacion){
        
        if(!guardarPrimerTrabajo){
            primerTrabajo=operacion;
            guardarPrimerTrabajo=true;            
        }            
        //Guardo la operacion en la matriz de maquinas
        matrizMaquinas[operacion->getNumMaquina()-1].push_back(operacion);     
        
        //Guardo en el historial la operacion
        colaOperaciones.push(operacion);
        
        //En caso de haber varias operaciones, las engancho
        numOperaciones=matrizMaquinas[operacion->getNumMaquina()-1].size();
        if(numOperaciones>1){
            matrizMaquinas[operacion->getNumMaquina()-1][numOperaciones-2]->setSucesor(matrizMaquinas[operacion->getNumMaquina()-1][numOperaciones-1]);
        }
        
        //Actualizo los tiempos:
        //Si la operacion depende de otra, se actualizan los tiempo dependiendo de su antecesora
        Nodo *antecesor=nodoDependiente(operacion);
        if((antecesor!=0)&&(tiempoMaquinas[operacion->getNumMaquina()-1]<antecesor->getTiempoMaxMaq())){
            operacion->setTiempoMinMaq(antecesor->getTiempoMaxMaq());
        }
        else
        {   //En caso contrario, depende de los tiempos de la maquina
            operacion->setTiempoMinMaq(tiempoMaquinas[operacion->getNumMaquina()-1]);
        }
        operacion->setTiempoMaxMaq(operacion->getTiempoMinMaq()+operacion->getTiempo());
        tiempoMaquinas[operacion->getNumMaquina()-1]=operacion->getTiempoMaxMaq();
        
    }
}


void Maquina::mostrarEstadoActual(){
    for(int i=0;i<matrizMaquinas.size();i++)
    {
        cout<<"-----------MAQUINA "<<i+1<<"------------"<<endl;
        for(int j=0;j<matrizMaquinas[i].size();j++){
            cout<<"Trabajo: "<<matrizMaquinas[i][j]->getID()<<endl;
    
        }
    }
    
}


void Maquina::limpiar(){
   
    this->primerTrabajo=0;
    matrizMaquinas.clear();
    matrizMaquinas.resize(numeroMaquinas);
    tiempoMaquinas.clear();
    tiempoMaquinas.resize(numeroMaquinas,0);
    guardarPrimerTrabajo=false;
}
/**
 * Devuelve el tiempo de la maquina que mas lleva 
 * consumido 
 *  
 */
int Maquina::getCMAX(){
    
    vector<int>::iterator it;
    vector<int>::iterator begin=tiempoMaquinas.begin();
    vector<int>::iterator end=tiempoMaquinas.end();    
    it=max_element(begin,end);
    return (*it);
    
}

/*
 * Devuelve el numero total de elementos de la matriz
 * NumMaquinas*NumTrabajos
 */
int Maquina::getTamMatriz(){
    if(!matrizMaquinas[0].empty())
        return matrizMaquinas.size()*matrizMaquinas[0].size();
    else
        return -1;
}
 

vector<vector<Nodo*> > Maquina::getMatrizMaquinas() const {
    return matrizMaquinas;
}

int Maquina::getNumMaquinas(){
    return numeroMaquinas;
}

int Maquina::getNumTrabajos(){
    return matrizMaquinas[0].size();
}
/*
 * A diferencia de insertar en maquina, este metodo, solo
 * guarda los trabajos sin realizar ningun calculo de tiempos
 * Solo almacena los predecesores
 */
void Maquina::guardarEnMaquina(Nodo* operacion){
    
    int numOperaciones=0;
    if(operacion){ 
        matrizMaquinas[operacion->getNumMaquina()-1].push_back(operacion);
        
        //En caso de haber varias operaciones, las engancho
        numOperaciones=matrizMaquinas[operacion->getNumMaquina()-1].size();
        if(numOperaciones>1){
            matrizMaquinas[operacion->getNumMaquina()-1][numOperaciones-2]->setSucesor(matrizMaquinas[operacion->getNumMaquina()-1][numOperaciones-1]);
        } 
    }
   
}

/*
 * En el caso de usar el constructor por defecto, se deben inicilizar aqui 
 * los valores
 */
void Maquina::inicializarMaquina(int numMaquinas){
    matrizMaquinas.resize(numMaquinas);
    tiempoMaquinas.resize(numMaquinas,0);
    this->guardarPrimerTrabajo=false;
    this->numeroMaquinas=numMaquinas;
    this->primerTrabajo=0;
   
}

/*
 * Cambia la operacion A por el B a la hora de insertarlas en la maquina
 * Los sucesores tambien quedan actualizados 
 */
void Maquina::cambiarTrabajo(Nodo *operacionA,Nodo *operacionB){
   /*
    * Debido a que todos los puenteros hacen referencia a los nodos del mismo grafo, hay que 
    * resetear sus valores de tiempos y poner los sucesores a 0
    */
    this->restablecerOperaciones();
   
    /*
     * Vacia la maquina de operaciones, para volver a rellenarla
     * NOTA: La cola de operaciones permanece constante!!!
     */
    this->limpiar();
    
    queue<Nodo*> colaAux;
     
    //Copio el orden en el que se tienen que insertar las operaciones en la colaAux
    while(!colaOperaciones.empty())    
    {        
       
        if(colaOperaciones.front()->getID()==operacionA->getID())
        {
            colaAux.push(operacionB);            
        }
        else
        {
            if(colaOperaciones.front()->getID()==operacionB->getID())
            {
                colaAux.push(operacionA);                           
            }
            else
            {
                colaAux.push(colaOperaciones.front());                 
            }
                    
        }
        colaOperaciones.pop();                
    }
    while(!colaAux.empty()){
        this->insertarEnMaquina(colaAux.front());
        colaAux.pop();
    } 
     
     
}

/*
 * Dada una cola de operaciones, restaura la maquina y la vuelve
 * a inicializar en el orden de las operaciones de la cola introducida
 */
void Maquina::restablecerMaquinaCola(queue<Nodo*> cola){
    
    this->restablecerOperaciones();
    this->limpiar();
    
    //Al no tenener una funcion clear(),tengo que borrar los elementos 1 a 1
    while(!colaOperaciones.empty())    
    { 
        colaOperaciones.pop();                
    }
    while(!cola.empty()){
        this->insertarEnMaquina(cola.front());
        cola.pop();
    } 
     
}



/*
 * Cambia de orden las operaciones A por B de la maquina indicada
 */
void Maquina::cambiarTrabajoPOS(int maquina, int trabajoA, int trabajoB){
     
    //Cambio el orden 
   
    Nodo *nodoAux=matrizMaquinas[maquina][trabajoA];
    matrizMaquinas[maquina][trabajoA]=matrizMaquinas[maquina][trabajoB];
    matrizMaquinas[maquina][trabajoB]=nodoAux;
 
    //Actualizo sucesores
   
    for(int i=0;i<matrizMaquinas[maquina].size();i++){        
        if(i==matrizMaquinas[maquina].size()-1)
            matrizMaquinas[maquina][i]->setSucesor(0);
        else
            matrizMaquinas[maquina][i]->setSucesor(matrizMaquinas[maquina][i+1]);
    }
}



/*
 * Devuelve la posicion en la que esta la operacion dentro 
 * de la matriz
 */
int Maquina::getPosicion(Nodo* operacion){
    int posicion=-1;
    for(int i=0;i<matrizMaquinas[operacion->getNumMaquina()-1].size();i++){
        if(matrizMaquinas[operacion->getNumMaquina()-1][i]->getID()==operacion->getID()){            
            posicion=i;
            break;
        }
            
    }
    
    return posicion;
}

/*
 * Resetea el estado de los nodos poniendo sus sucesores a 0
 * y sus tiempos de maquina minimos al 0 y maximos=tiempoNodo()
 */
void Maquina::restablecerOperaciones(){
    
    for(int i=0;i<matrizMaquinas.size();i++){
        for(int j=0;j<matrizMaquinas[i].size();j++){
            matrizMaquinas[i][j]->setTiempoMaxMaq(matrizMaquinas[i][j]->getTiempo());
            matrizMaquinas[i][j]->setTiempoMinMaq(0);
            matrizMaquinas[i][j]->setSucesor(0);
        }
    }
        
}


/*
 * Copia los valores de la maquina que se le pasa como parametro
 * a la maquina actual 
 */
void Maquina::copiarMaquina(Maquina& orig){
    this->matrizMaquinas=orig.matrizMaquinas;
    this->numeroMaquinas=orig.numeroMaquinas;
    this->primerTrabajo=orig.primerTrabajo;
    this->tiempoMaquinas=orig.tiempoMaquinas;
    this->guardarPrimerTrabajo=orig.guardarPrimerTrabajo;
    this->colaOperaciones=orig.colaOperaciones;
}

queue<Nodo*> Maquina::getColaOperaciones() const {
    return colaOperaciones;
}
