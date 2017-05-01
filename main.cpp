/* 
 * File:   main.cpp
 * Author: Drebin
 *
 * Created on 8 de noviembre de 2015, 10:24
 */

#include <cstdlib>
#include <iostream> 
#include <vector> 
#include <algorithm>
#include <queue>
#include <functional>
#include <map>
#include <memory>
#include <tr1/memory>
#include "Grafo.h"
#include "Nodo.h"
#include "Maquina.h"
#include "AGGE.h"
#include "Timer.h"
#include "BusquedaLocal.h"

using namespace std;

//Semilla para generar los numeros aleatorios
int rdtsc()
{
    __asm__ __volatile__("rdtsc");
}


int AlgoritmoGT(Grafo &grafo,Maquina &maquina){
    
//    cout<<"--------------------- ALGORITMO GT-----------------"<<endl;
    
    vector<Nodo*> planificables;
    vector<vector<Nodo*> > conflictos;
    conflictos.resize(grafo.GetNumMaquinas());  
    int numConflictos=0;
    int numMaquina=-1;
    vector<Nodo*>::iterator it;
    
    //Voy insertando las operaciones iniciales de cada trabajo
    for(int i=0;i<grafo.GetVectorNodos()[0].getSucesores().size();i++){
        planificables.push_back(grafo.GetVectorNodos()[0].getSucesores()[i]);
    }
    
    while(!planificables.empty()){
    
        //Rellenamos el vector de conflictos
        for(int i=0;i<planificables.size();i++){
            //En las posiciones de las maquinas se le resta 1 para no desperdiciar memoria del vector
            conflictos[planificables[i]->getNumMaquina()-1].push_back(planificables[i]);    
        }

        numConflictos=0;
        numMaquina=-1;
        for(int i=0;i<conflictos.size();i++){
            if(conflictos[i].size()>numConflictos)
            {
                numConflictos=conflictos[i].size();
                numMaquina=i;//La posicion del vector NO corresponde con el numero de la maquina
            }
        }

        //Elegimos de forma aleatoria una operacion del conjunto de conflictos
        srand(rdtsc());//Semilla
        Nodo *operacion=conflictos[numMaquina][rand()%conflictos[numMaquina].size()];

        //La insertamos en la maquina
        maquina.insertarEnMaquina(operacion);
        
        //Cambio la operacion que he quitado por su operacion siguiente en 
        //el caso de tenerla
        it=planificables.begin();
        for(int i=0;i<planificables.size();i++){
            if(planificables[i]->getID()==operacion->getID()){

                if(planificables[i]->getSiguiente()!=0){                    
                    planificables[i]=planificables[i]->getSiguiente();                
                }
                else
                {   //En caso de no tener siguiente la quito del conjunto Q                  
                    planificables.erase(it);                
                }
                break;
            }    
            it++;
        }
        
        //Restauramos el vector de conflictos
        conflictos.clear();
        conflictos.resize(grafo.GetNumMaquinas());    
    }
    
   
    return maquina.getCMAX();
}


/*
 * Si el 90% o mas de la poblacion es la misma, devuelve true
 */
bool reinicializar(Maquina *poblacion[],int tamPoblacion){
   
    map<int,int> mapaCmax;
    
    //Guardamos en el mapa las veces que se repite cada valor que sea distinto
    for(int i=0;i<tamPoblacion;i++){
        mapaCmax[poblacion[i]->getCMAX()]+=1;
    }
    
    //Almacenamos cual es el valor que mas veces se repite
    map<int,int>::iterator it=mapaCmax.begin();
    int numRepetidos=(*it).second;
    while(it!=mapaCmax.end()){
       
        if((*it).second>numRepetidos)
            numRepetidos=(*it).second;
        it++;
    }
    
    //Si el numero de repetidos representa mas de un 90% devuelve true
    if(((double)numRepetidos/(double)tamPoblacion)>=0.9)
        return true;
    else
        return false;
    
}

void aplicarReinicializacion(Maquina *vectorMaquina[],Grafo *vectorGrafo[],int tamPoblacion,int mejorCMAX){
   
    bool mejorIndividuo=false;
    
    for(int i=0;i<tamPoblacion;i++){
        //En caso de no mejorar, dejo a los mejores y cambio el resto usando el GT
        if(vectorMaquina[i]->getCMAX()!=mejorCMAX || mejorIndividuo){

            vectorMaquina[i]->restablecerOperaciones();
            vectorMaquina[i]->limpiar();
            AlgoritmoGT(*(vectorGrafo[i]),*(vectorMaquina[i]));

        }
        else
        {
            mejorIndividuo=true;//Permite dejar solamente a 1 mejor individuo
        }
    }
}


/*
 * Cada 10 generaciones se aplica BL sobre todos los cromosomas
 */
void aplicarAM10_10(Maquina *vectorMaquina[],Grafo *vectorGrafo[],int numeroIteraciones,int numeroPadres,int cmaxOptimo)
{
    
    Timer tiempo;
    priority_queue<int,vector<int>,greater<int> > mejorCMAX;
    int numeroGeneraciones=0;
    int contadorReiniciar=0;
    int mejorCMAX_anterior=99999;//Establecemos un rango alto para que las comparaciones las realice de forma correcta
    int mejorCMAX_Generico=99999;
  
    tiempo.start();
    for(int i=0;i<numeroIteraciones;i++){
        
        //Si el 90% de la poblacion es la misma, reinicilizamos
        if(reinicializar(vectorMaquina,numeroPadres)){
           
            aplicarReinicializacion(vectorMaquina,vectorGrafo,numeroPadres,mejorCMAX_Generico);
            
        }
       
        
         unique_ptr<AGGE> algoritmoGeneracional(new AGGE(numeroPadres,vectorMaquina[0]->getNumMaquinas(),vectorMaquina[0]->getNumTrabajos()));
         algoritmoGeneracional.get()->aplicarAlgoritmoAGGE(vectorMaquina,vectorGrafo);
        
        //Cada 10 generaciones aplico la BL sobre todos los cromosomas
        if(numeroGeneraciones==10)
        {   
            for(int j=0;j<numeroPadres;j++){
                
                unique_ptr<BusquedaLocal> dynamicPointerBL(new BusquedaLocal);
                dynamicPointerBL.get()->aplicarBusquedaLocal(vectorMaquina[j],vectorGrafo[j]);
                i++;//Cada vez que se hace uso de BL, cuenta como evaluacion
            }
            
            numeroGeneraciones=0;
           
        }            
                  
        for(int j=0;j<numeroPadres;j++){
            mejorCMAX.push(vectorMaquina[j]->getCMAX());            
        }
        
        //Por cada iteracion, aumento el numero de generaciones
        numeroGeneraciones++;
        
        //Establezco por cada iteracion al mejor cmax obtenido hasta el momento
        if(mejorCMAX.top()<mejorCMAX_Generico){
            mejorCMAX_Generico=mejorCMAX.top();
        }
        
        //En caso de encontrar el mejor CMAX, paro de buscar
        if(mejorCMAX.top()==cmaxOptimo)
            break;
        
        //A las 20 iteraciones, realizamos la comprobacion de la reinicializacion
        if(contadorReiniciar==20){
            
            //Si la poblacion mejora, se establece el nuevo mejor CMAX
            if(mejorCMAX.top()<mejorCMAX_anterior){               
                mejorCMAX_anterior=mejorCMAX.top();
            }
            else{
            //En caso de que la poblacion no mejore    
              
                //En caso de no mejorar, dejo a los mejores y cambio el resto usando el GT                                      
                aplicarReinicializacion(vectorMaquina,vectorGrafo,numeroPadres,mejorCMAX_Generico);
                
            }                    
            contadorReiniciar=0;
        }
        
        
        //Aumentamos la reinicializacion
        contadorReiniciar++;
       
        
        //Vaciar cola
        while(!mejorCMAX.empty()){
            mejorCMAX.pop();
        }
     cout<<"Mejor cmax_prov: "<<mejorCMAX_Generico<<endl;   
    }
    tiempo.stop();
    
    cout<<"TIEMPO :"<<tiempo.getElapsedTimeInSec()<<endl;
    cout<<"El mejor cmax es: "<<mejorCMAX_Generico<<endl;
       
}

 

/*
 * Cada 10 generaciones se aplica busqueda local a un individuo de la poblacion con una probabilidad de 1%
 */
void aplicarAM10_01(Maquina *vectorMaquina[],Grafo *vectorGrafo[],int numeroIteraciones,int numeroPadres,int cmaxOptimo){    
      
    Timer tiempo;
    priority_queue<int,vector<int>,greater<int> > mejorCMAX;
    int numeroGeneraciones=0;
    float probabilidad=1;
    int contadorReiniciar=0;
    int mejorCMAX_anterior=99999;//Establecemos un rango alto para que las comparaciones las realice de forma correcta
    int mejorCMAX_Generico=99999;
  
    tiempo.start();
    for(int i=0;i<numeroIteraciones;i++){
              
          //Si el 90% de la poblacion es la misma, reinicilizamos
        if(reinicializar(vectorMaquina,numeroPadres)){           
            aplicarReinicializacion(vectorMaquina,vectorGrafo,numeroPadres,mejorCMAX_Generico);            
        }
        
        
         unique_ptr<AGGE> algoritmoGeneracional(new AGGE(numeroPadres,vectorMaquina[0]->getNumMaquinas(),vectorMaquina[0]->getNumTrabajos()));
         algoritmoGeneracional.get()->aplicarAlgoritmoAGGE(vectorMaquina,vectorGrafo);
        
        //Cada 10 generaciones aplico la BL 
        if(numeroGeneraciones==10)
        {   srand(rdtsc());
            for(int j=0;j<numeroPadres;j++){
                //Por cada padre,se aplica la probabilidad de ser seleccionado para aplicarle BL
                if(rand()%100<probabilidad)
                {
                    unique_ptr<BusquedaLocal> dynamicPointerBL(new BusquedaLocal);
                    dynamicPointerBL.get()->aplicarBusquedaLocal(vectorMaquina[j],vectorGrafo[j]);
                    i++;//Cada vez que se hace uso de BL, cuenta como evaluacion
                }
            }
           
            numeroGeneraciones=0;
        }            
       
        for(int j=0;j<numeroPadres;j++){
            mejorCMAX.push(vectorMaquina[j]->getCMAX());            
        }
        
        //Por cada iteracion, aumento el numero de generaciones
        numeroGeneraciones++;
        
        //Establezco por cada iteracion al mejor cmax obtenido hasta el momento
        if(mejorCMAX.top()<mejorCMAX_Generico){
            mejorCMAX_Generico=mejorCMAX.top();
        }
                          
        //En caso de encontrar el mejor CMAX, paro de buscar
        if(mejorCMAX.top()==cmaxOptimo)
            break;
        
         //A las 20 iteraciones, realizamos la comprobacion de la reinicializacion
        if(contadorReiniciar==20){
            
            //Si la poblacion mejora, se establece el nuevo mejor CMAX
            if(mejorCMAX.top()<mejorCMAX_anterior){               
                mejorCMAX_anterior=mejorCMAX.top();
            }
            else{
            //En caso de que la poblacion no mejore    
              
                //En caso de no mejorar, dejo a los mejores y cambio el resto usando el GT                                      
                aplicarReinicializacion(vectorMaquina,vectorGrafo,numeroPadres,mejorCMAX_Generico);
                
            }                    
            contadorReiniciar=0;
        }
        
        
        //Aumentamos la reinicializacion
        contadorReiniciar++;
       
        
        //Vaciar cola
        while(!mejorCMAX.empty()){
            mejorCMAX.pop();
        }
        
    }
    tiempo.stop();
    
    cout<<"TIEMPO :"<<tiempo.getElapsedTimeInSec()<<endl;
    cout<<"El mejor cmax es: "<<mejorCMAX_Generico<<endl;
    
}



/*
 * Cada 10 generaciones se aplica BL a los 0.1*N mejores individuos
 */
void aplicarAM10_01Mej(Maquina *vectorMaquina[],Grafo *vectorGrafo[],int numeroIteraciones,int numeroPadres,int cmaxOptimo){
     
    class Compare{
    public:
        bool operator()(pair<Maquina*,int> p1,pair<Maquina*,int> p2){
            return p1.first->getCMAX()>p2.first->getCMAX();
        }
    };
    
    
    Timer tiempo;
    priority_queue<int,vector<int>,greater<int> > mejorCMAX;
    priority_queue<pair<Maquina*,int>,vector<pair<Maquina*,int> >,Compare> mejoresMaquinas;
    pair<Maquina*,int> parAux;
    int numeroGeneraciones=0;
    int numCromosomasBL=(int)0.1*numeroPadres;
     int contadorReiniciar=0;
    int mejorCMAX_anterior=99999;//Establecemos un rango alto para que las comparaciones las realice de forma correcta
    int mejorCMAX_Generico=99999;
  
    tiempo.start();
    for(int i=0;i<numeroIteraciones;i++){
              
        //Si el 90% de la poblacion es la misma, reinicilizamos
        if(reinicializar(vectorMaquina,numeroPadres)){
           
            aplicarReinicializacion(vectorMaquina,vectorGrafo,numeroPadres,mejorCMAX_Generico);
            
        }
        
        unique_ptr<AGGE> algoritmoGeneracional(new AGGE(numeroPadres,vectorMaquina[0]->getNumMaquinas(),vectorMaquina[0]->getNumTrabajos()));
        algoritmoGeneracional.get()->aplicarAlgoritmoAGGE(vectorMaquina,vectorGrafo);
                  
        //Cada 10 generaciones aplico la BL 
        if(numeroGeneraciones==10)
        {  
            //Cargo las mejores maquinas
            for(int j=0;j<numeroPadres;j++)
            {
                parAux=make_pair(vectorMaquina[j],j);                            
                mejoresMaquinas.push(parAux);
            }
                       
            for(int j=0;j<numCromosomasBL;j++){
                
                unique_ptr<BusquedaLocal> dynamicPointerBL(new BusquedaLocal);
                dynamicPointerBL.get()->aplicarBusquedaLocal(vectorMaquina[mejoresMaquinas.top().second],vectorGrafo[mejoresMaquinas.top().second]);
                mejoresMaquinas.pop();  
                i++;//Cada vez que se hace uso de BL, cuenta como evaluacion
            }                  
            //Vacio la cola
            while(!mejoresMaquinas.empty())
                mejoresMaquinas.pop();
            
            numeroGeneraciones=0;
        }            
       
        for(int j=0;j<numeroPadres;j++){
            mejorCMAX.push(vectorMaquina[j]->getCMAX());            
        } 
        
        //Por cada iteracion, aumento el numero de generaciones
        numeroGeneraciones++;
                  
         //Establezco por cada iteracion al mejor cmax obtenido hasta el momento
        if(mejorCMAX.top()<mejorCMAX_Generico){
            mejorCMAX_Generico=mejorCMAX.top();
        }
          
        
        //En caso de encontrar el mejor CMAX, paro de buscar
        if(mejorCMAX.top()==cmaxOptimo)
            break;
        
        //A las 20 iteraciones, realizamos la comprobacion de la reinicializacion
        if(contadorReiniciar==20){
            
            //Si la poblacion mejora, se establece el nuevo mejor CMAX
            if(mejorCMAX.top()<mejorCMAX_anterior){               
                mejorCMAX_anterior=mejorCMAX.top();
            }
            else{
            //En caso de que la poblacion no mejore    
              
                //En caso de no mejorar, dejo a los mejores y cambio el resto usando el GT                                      
                aplicarReinicializacion(vectorMaquina,vectorGrafo,numeroPadres,mejorCMAX_Generico);
                
            }                    
            contadorReiniciar=0;
        }
        
        
        //Aumentamos la reinicializacion
        contadorReiniciar++;
       
        
        //Vaciar cola
        while(!mejorCMAX.empty()){
            mejorCMAX.pop();
        }
        
        
    }
    tiempo.stop();
    
    cout<<"TIEMPO :"<<tiempo.getElapsedTimeInSec()<<endl;
    cout<<"El mejor cmax es: "<<mejorCMAX_Generico<<endl;
}



int main(int argc, char** argv) {
       
    Timer tiempo;
    string nombreFichero="swv01.txt";   
    int numeroPadres=20; //Tamaño del la poblacion=20
    int numeroIteraciones=20000; //Numero de evaluaciones=20000
    Grafo *vectorGrafo[numeroPadres];
    Maquina *vectorMaquina[numeroPadres];
//    priority_queue<int,vector<int>,greater<int> > mejorCMAX;
   
//    tiempo.start();
    for(int i=0;i<numeroPadres;i++){
               
        vectorGrafo[i]=new Grafo(nombreFichero);
        vectorMaquina[i]=new Maquina(vectorGrafo[i]->GetNumMaquinas());        
        AlgoritmoGT(*(vectorGrafo[i]),*(vectorMaquina[i]));
//        mejorCMAX.push(vectorMaquina[i]->getCMAX());
                
    }
//    tiempo.stop();
//    cout<<"TIEMPO :"<<tiempo.getElapsedTimeInSec()<<endl;
//    cout<<"El mejor cmax es: "<<mejorCMAX.top()<<endl;
    
    
    //aplicarAM10_10(vectorMaquina,vectorGrafo,numeroIteraciones,numeroPadres,vectorGrafo[0]->GetTiempoOptimo());
    
    //aplicarAM10_01(vectorMaquina,vectorGrafo,numeroIteraciones,numeroPadres,vectorGrafo[0]->GetTiempoOptimo());
        
    aplicarAM10_01Mej(vectorMaquina,vectorGrafo,numeroIteraciones,numeroPadres,vectorGrafo[0]->GetTiempoOptimo());
    
    
    
    //Borrar las reservas de memorias
    for(int i=0;i<numeroPadres;i++){
        delete vectorGrafo[i];
        delete vectorMaquina[i];
    }
 
    return 0;
}

