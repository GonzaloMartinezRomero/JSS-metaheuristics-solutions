/* 
 * File:   AGGE.cpp
 * Author: Drebin
 * 
 * Created on 12 de noviembre de 2015, 18:34
 */

#include "AGGE.h"

AGGE::AGGE(int _tamPoblacion,int _numMaquinas,int _numTrabajos) {
    this->tamPoblacion=_tamPoblacion;
    this->hijos.resize(tamPoblacion);
    this->numMaquinas=_numMaquinas;
    this->numTrabajos=_numTrabajos;
   
    
    //Inicializo los hijos    
    for(int i=0;i<tamPoblacion;i++){
        hijos[i].inicializarMaquina(numMaquinas);
    }
   
    //N=Pc*(M/2) Numero de parejas que se van a cruzar
    double formula=0.7*(tamPoblacion/2);
    this->numCromosomasCruce=floor(formula+0.5);    
//    cout<<"Num cromosomas para cruce: "<< numCromosomasCruce<<endl;
}


AGGE::~AGGE() {
}

/*
 * Para hacer la seleccion, escogemos a toda la poblacion
 */
void AGGE::seleccion(Maquina *poblacion[]){
/* Nota: Aunque los padres y los hijos usen maquinas distintas, las operaciones con las que
 * estan trabajando, operan sobre los mismos nodos. Los tiempos de esos nodos han sido ya modificados
 * por el algoritmo GT, por lo que, para que los tiempos a la hora de volverlos a insertar en la maquina
 * sean correctos se tienene que restablecer, por eso se utiliza la funcion restablecerOperaciones*/    
    
    for(int i=0;i<tamPoblacion;i++){
        padres.push_back(poblacion[i]);
        padres[i]->restablecerOperaciones();
    }    
}

/*Semilla para generar los numeros aleatorios*/
int AGGE::rdtsc()
{
    __asm__ __volatile__("rdtsc");
}


int AGGE::buscarMejorMaquina(Maquina* poblacion[]){
    
    int cmax=poblacion[0]->getCMAX();
    mejorMaquina=*poblacion[0];
    
    for(int i=1;i<tamPoblacion;i++){
        if(poblacion[i]->getCMAX()<cmax){
            cmax=poblacion[i]->getCMAX();
            mejorMaquina=*poblacion[i];
        }
    }
    return cmax;
    
}


/*
 * Devuelve el indice que indica el comienzo de la subsecuencia
 * contenida en el padre2
 * Devuelve -1 si no la ha encontrado
 */
int AGGE::calcularSubSecuencia(Nodo* subSecuencia[], Nodo* padre[],int tamSubSecuencia,int numOperaciones){
    
    int indice2=0;
    int contador=0;    
      
    while((indice2+tamSubSecuencia-1)<(numOperaciones)){
       
        for(int i=0;i<tamSubSecuencia;i++){  
            
            for(int j=0;j<tamSubSecuencia;j++){
                //OJO! Al comparar lo haces con las id
                if(padre[indice2+i]->getID()==subSecuencia[j]->getID()){
                    contador++;                   
                }                    
            }                       
        }
        if(contador==tamSubSecuencia){           
            return indice2;
        }
        else{            
            contador=0;
        }        
        indice2++;
    }
    return -1;
    
}

void AGGE::SXX(Nodo *padre1[],Nodo *padre2[],int numOperaciones,int indicePadres){  
    
    bool encontrada=false;
    Nodo *hijo1[numOperaciones];
    Nodo *hijo2[numOperaciones]; 
    
    //Calcular el tamaño de la subsecuencia
    int tamSubSecuencia=(numOperaciones/2);
    //Establecer min=2 y max=5
    if(tamSubSecuencia<2)
        tamSubSecuencia=2;
    else{
        if(tamSubSecuencia>5)
            tamSubSecuencia=5;
    }
    
    int indice1=0;
    int indice2=0;
    Nodo *subSecuencia[tamSubSecuencia];
    
    while(!encontrada){
       
        for(int i=0;i<tamSubSecuencia;i++){
            subSecuencia[i]=padre1[indice1+i];            
        }
       
        indice2=calcularSubSecuencia(subSecuencia,padre2,tamSubSecuencia,numOperaciones);
        
        //Ha encontrado la subsecuencia
        if(indice2!=-1){
            encontrada=true;
            //Generar los hijos con las subsecuencias cambiadas     
            //Generar el hijo1
            for(int i=0;i<numOperaciones;i++){
                if(i==indice1){
                    for(int j=0;j<tamSubSecuencia;j++){
                        hijo1[i]=padre2[indice2+j];
                        i++;
                    }
                
                }
                //Al terminar de copiar, puede darse el caso de haber copiado los ultimos
                //elementos y la i, apunta a una zona invalida
                if(i<numOperaciones)
                    hijo1[i]=padre1[i];
            }
            
            //Generar el hijo2
            for(int i=0;i<numOperaciones;i++){
                if(i==indice2){
                    for(int j=0;j<tamSubSecuencia;j++){
                        hijo2[i]=padre1[indice1+j];
                        i++;
                    }
                
                }
                //Al terminar de copiar, puede darse el caso de haber copiado los ultimos
                //elementos y la i, apunta a una zona invalida
                if(i<numOperaciones)
                    hijo2[i]=padre2[i];
            }
            
        }
        else
        {
            //Comprobar que en el caso de avanzar a la siguiente secuencia no 
            //excede el tamaño del vector
            if(indice1+1+tamSubSecuencia<numOperaciones)
                indice1+=1;
            else{
                //Se sobrepasa el limite del vector y se reduce en 1 el tam de la subsecuencia
                //Si la subsecuencia ha llegado al minimo, acaba la ejecucion
                if(tamSubSecuencia-1<2){
                    encontrada=true;
                    //Realizo las copias sin llegar a realizar el cruce    
                    for(int i=0;i<numOperaciones;i++){
                        
                        hijo1[i]=padre1[i];
                        hijo2[i]=padre2[i];
                    }
                }
                    
                else
                {
                    tamSubSecuencia--;
                    indice1=0;
                }
                
            }
        }
        
   }
   //Cuando acabamos, guardamos los hijos en su maquina correspondiente
   //Nota: Se puede hacer a la hora de insertarlo en los padres   
  
    for(int i=0;i<numOperaciones;i++){     
               
        hijos[indicePadres].guardarEnMaquina(hijo1[i]);
        hijos[indicePadres+1].guardarEnMaquina(hijo2[i]);
    }
    
    
    
}

void AGGE::cruce(){
    
    //Ir leyendo los padres de 2 en 2
    //Tan solo se van a cruzar un determinado numero de padres    
    Nodo *padre1[numTrabajos];
    Nodo *padre2[numTrabajos];    
    
    //Cuando cruzan, no cruzan todos, solo cruzan los numCromosomas
    int indice=0;
    for(indice;indice<numCromosomasCruce;indice+=2){        
        for(int j=0;j<numMaquinas;j++){
            for(int k=0;k<numTrabajos;k++){
                
                padre1[k]=padres[indice]->getMatrizMaquinas()[j][k];               
                padre2[k]=padres[indice+1]->getMatrizMaquinas()[j][k];                  
            }
            
            SXX(padre1,padre2,numTrabajos,indice);
            
        }        
    }
     
    /*El resto de hijos que no han sido cruzados se copian igual que los padres*/
    for(indice;indice<hijos.size();indice++)
    {
        for(int j=0;j<padres[indice]->getNumMaquinas();j++)
        {            
            for(int k=0;k<padres[indice]->getNumTrabajos();k++)
            {
                hijos[indice].guardarEnMaquina(padres[indice]->getMatrizMaquinas()[j][k]);
            }
        }      
    }
    
       
}

void AGGE::mutacion(){
    int numEsperadoMutaciones=1*(tamPoblacion*0.02);
    int maquina=0;    
    int trabajoA=0;
    int trabajoB=0;
    int desplazamiento=0;
    
    
    for(int i=0;i<numEsperadoMutaciones;i++){
        srand(rdtsc());//Semilla
        maquina=rand()%numMaquinas;
        
        //Se eligen 2 trabajos
        while(trabajoA==trabajoB){
            srand(rdtsc());//Semilla
            trabajoA=rand()%numTrabajos;
            
            srand(rdtsc());//Semilla
            trabajoB=rand()%numTrabajos;
        }
        
        srand(rdtsc());//Semilla
        desplazamiento=rand()%numTrabajos;
        
              hijos[rand()%tamPoblacion].cambiarTrabajoPOS(maquina,trabajoA,trabajoB);
    }
    
    
    
}

void AGGE::reparacion(int indiceHijo,Grafo *grafo[]){
    
    Maquina maquinaAux(numMaquinas);
    vector<int> vectorIndices;
    vectorIndices.resize(numMaquinas,0);
    
    vector<Nodo*> planificables;
        
     //Voy insertando las operaciones iniciales de cada trabajo
    for(int i=0;i<grafo[indiceHijo]->GetVectorNodos()[0].getSucesores().size();i++){
        planificables.push_back(grafo[indiceHijo]->GetVectorNodos()[0].getSucesores()[i]);
    }
     
    
    vector<Nodo*>::iterator it;
    vector<vector<Nodo*> >conflictos;
    conflictos.resize(numMaquinas);
     
  
    int numConflictos=0;
    int numMaquina=-1; 
    bool insertado=false;
    
    while(!planificables.empty()){
        
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
                numMaquina=i;
            }
        }
         
        
        Nodo *nodoAux=0;
        insertado=false;
        
        while(!insertado){
            for(int i=0;i<conflictos[numMaquina].size();i++){

                nodoAux=conflictos[numMaquina][i];        
                if(nodoAux->getID()==hijos[indiceHijo].getMatrizMaquinas()[nodoAux->getNumMaquina()-1][vectorIndices[nodoAux->getNumMaquina()-1]]->getID())
                {
                   
                    maquinaAux.insertarEnMaquina(nodoAux);                   
                    vectorIndices[nodoAux->getNumMaquina()-1]+=1;
                    insertado=true;           
                    break;
                }
            }

            //En caso de no haber sido insertado se tiene que cambiar el orden
            //en la maquina de los hijos
            int valorMin=nodoAux->getTiempoMaxMaq();
            if(!insertado){
                
                //Busco el nodo con el menor cmax
                for(int i=0;i<conflictos[numMaquina].size();i++){
                    
                    if(conflictos[numMaquina][i]->getTiempoMaxMaq()<valorMin){
                       
                        valorMin=conflictos[numMaquina][i]->getTiempoMaxMaq();
                        nodoAux=conflictos[numMaquina][i];
                        
                    }

                }
               
                  hijos[indiceHijo].cambiarTrabajoPOS(nodoAux->getNumMaquina()-1,vectorIndices[nodoAux->getNumMaquina()-1],hijos[indiceHijo].getPosicion(nodoAux));
                    
            }
        }

        //Se actualiza el conjunto Q
        it=planificables.begin();
        for(int i=0;i<planificables.size();i++){
            if(planificables[i]->getID()==nodoAux->getID()){

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
      conflictos.resize(numMaquinas);    
      
}
    //Sustituyo el nuevo estado de la maquina
    hijos[indiceHijo]=maquinaAux;
   
}

void AGGE::aplicarAlgoritmoAGGE(Maquina* poblacion[],Grafo *grafo[]){
    
    int mejorCMAX=buscarMejorMaquina(poblacion);
    seleccion(poblacion);
    cruce();
    mutacion();   
    
    /*
     * La reparacion se tiene que hacer sobre los hijos que han pasado por el cruce+mutacion
     * el resto no se tiene que reparar!!
     */
    for(int i=0;i<hijos.size();i++){
       reparacion(i,grafo);        
    }
    
    //Intercambio el mejor padre por el peor hijo
    int cmax=hijos[0].getCMAX();
    int peorHijo=0;    
    int cmaxAux=0;
    
    for(int i=1;i<hijos.size();i++){
        cmaxAux=hijos[i].getCMAX();
        if(cmaxAux>cmax){
            cmax=cmaxAux;
            peorHijo=i;
        }
            
    }
            
    
    //Aplicamos el elitismo
    if(cmax>mejorCMAX){
        hijos[peorHijo]=mejorMaquina;
        reparacion(peorHijo,grafo);    
    }
        

    //Reemplazar la poblacion anterior
    for(int i=0;i<tamPoblacion;i++){
        poblacion[i]->copiarMaquina(hijos[i]);
    }
     
   
    
}

