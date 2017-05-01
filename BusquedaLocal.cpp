/* 
 * File:   BusquedaLocal.cpp
 * Author: Drebin
 * 
 * Created on 29 de noviembre de 2015, 12:25
 */

#include "BusquedaLocal.h"

BusquedaLocal::BusquedaLocal() {

}


BusquedaLocal::~BusquedaLocal() {}

/*
 * Dado el conjunto de nodos pertenecientes al camino critico, devuelve
 * una estructura que contiene a las maquinas y las operaciones que pueden
 * ser intercambiadas
 */
vector<vector<Nodo*> > BusquedaLocal::generarBloquesCriticos(list<Nodo*> &nodosCaminoCritico,int numeroMaquinas){
    
    vector<vector<Nodo*> > bloquesCriticos(numeroMaquinas);
    
    list<Nodo*>::iterator it1=nodosCaminoCritico.begin();
    list<Nodo*>::iterator it2=it1;
    it2++;
    
    int numeroMaquina=-1;
    
    while(it2!=nodosCaminoCritico.end()){
        
        //Numero de maquina-1 para insertar correctamente en el vector
        if((*it1)->getNumMaquina()==(*it2)->getNumMaquina()){
            
            numeroMaquina=(*it1)->getNumMaquina()-1;
            
            bloquesCriticos[numeroMaquina].push_back((*it1));
            bloquesCriticos[numeroMaquina].push_back((*it2));
            it2++;
            
            while((it2!=nodosCaminoCritico.end())&&((*it2)->getNumMaquina()-1==numeroMaquina)){
                  bloquesCriticos[numeroMaquina].push_back((*it2));
                  it2++;
            }
            
        }
        if(it2!=nodosCaminoCritico.end()){
            it1=it2;
            it2++;
        }
       
    }
    
        
    return bloquesCriticos;
}



int BusquedaLocal::aplicarBusquedaLocal(Maquina *maquina,Grafo *grafo){
    int cmaxInicial=maquina->getCMAX();
    grafo->restablecerGrafo(maquina->getMatrizMaquinas());
    
    //Obtener los nodos del camino critico
    list<Nodo*> nodosCaminoCritico;
    nodosCaminoCritico=grafo->nodosCaminoCritico(maquina->getPrimerTrabajo());
            
    //Obtener los bloques criticos
    vector<vector<Nodo*> > bloquesCriticos=generarBloquesCriticos(nodosCaminoCritico,maquina->getNumMaquinas());
     
    int nuevoCmax=cmaxInicial;
    bool encontrado=false;
    
    //Guardo el estado inicial de la cola de operaciones para que en caso de generar
    //un cmax peor, pueda restaurar al estado inicial
    queue<Nodo*> colaOperaciones=maquina->getColaOperaciones();    
 
    //Iniciamos el bucle en el que vamos explorando todos los vecinos   
    for(int i=0;i<bloquesCriticos.size();i++){

        for(int j=0;j<((int)(bloquesCriticos[i].size())-1);j++){
           
            //Aplicar el operador de nowicki          
            maquina->cambiarTrabajo(bloquesCriticos[i][j],bloquesCriticos[i][j+1]);
                       
            nuevoCmax=maquina->getCMAX();
             
            //En caso de haberse encontrado, la poblacion entrante queda modificada
            if(nuevoCmax<cmaxInicial){
                encontrado=true;
                break;
            }
            else
            {
                //En caso de no encontrarse mejora, la poblacion se restaura a como estaba antes                 
                maquina->restablecerMaquinaCola(colaOperaciones);         
            }
        }
        if(encontrado)
            break;
    }       
  return nuevoCmax;
}