/* 
 * File:   Grafo.cpp
 * Author: Drebin
 * 
 * Created on 9 de noviembre de 2015, 9:37
 */

#include "Grafo.h"

Grafo::Grafo(const string _nombreFichero) {
    
    this->nombreFichero=_nombreFichero;
    
    
    ifstream fichEntrada;
    string varString;    
    int aux=0;
    int numNodos=0;
    int indice=1;//El indice tiene que empezar en 1 porque la pos 0 se reserva al origen
    
    
    fichEntrada.open(nombreFichero.c_str());
    
    //Lee el numero de trabajos
    getline(fichEntrada,varString);
    numTrabajos=atoi(varString.c_str());
    
    //Lee el numero de maquinas
    getline(fichEntrada,varString);
    numMaquinas=atoi(varString.c_str());
    
    //Lee el tiempo optimo
    getline(fichEntrada,varString);
    tiempoOptimo=atoi(varString.c_str());
        
    numNodos=(numTrabajos*numMaquinas)+1;
    
    //EEDD en la que se almacenan los nodos
    vectorNodos.resize(numNodos);
    
            
    //Se va rellenando el grafo con el numTrabajo y el tiempo
    for(int i=0;i<numTrabajos;i++){
        vectorNodos[0].anadirSucesor(&vectorNodos[indice]);//El nodo 0 se considera el inicial
        
        for(int j=0;j<numMaquinas;j++){
            if(j==numMaquinas-1){
                getline(fichEntrada,varString);//Lee el ultimo caracter del fichero
                vectorNodos[indice].setFinal(true);
            }
            else
            {
                getline(fichEntrada,varString,' ');//Lee los caracteres intermedios
            }   

            aux=atoi(varString.c_str());                
            vectorNodos[indice].setTiempo(aux);
            vectorNodos[indice].setNumTrabajo(i+1);
            vectorNodos[indice].setID(indice);
            indice++;

        }
        
    }
    
  
    //Rellena el grafo con las maquinas asociadas a cada trabajo
    indice=1;
    for(int i=0;i<numTrabajos;i++){
       
      for(int j=0;j<numMaquinas;j++){
          if(j==numMaquinas-1){
              getline(fichEntrada,varString);
          }
          else
          {
              getline(fichEntrada,varString,(' '));
          }  
       
        aux=atoi(varString.c_str());              
        vectorNodos[indice].setNumMaquina(aux);
        indice++;
          
      }
    }
    
    
    
    //Se establecen los sucesores directos. Los que no se pueden modificar
    indice=1;
    for(int i=0;i<numTrabajos;i++){
     
       for(int j=0;j<numMaquinas;j++){
          
            if((j+1)< numMaquinas){
                vectorNodos[indice].setSiguiente(&vectorNodos[indice+1]);                  
            }
            indice++;    
       }          
      }
    
}

Grafo::Grafo(){
    this->nombreFichero=" ";
    this->numMaquinas=0;
    this->numTrabajos=0;
    this->tiempoOptimo=0;
   
}

Grafo::~Grafo() {
}

void Grafo::SetTiempoOptimo(int tiempoOptimo) {
    this->tiempoOptimo = tiempoOptimo;
}

int Grafo::GetTiempoOptimo() const {
    return tiempoOptimo;
}

void Grafo::SetNumMaquinas(int numMaquinas) {
    this->numMaquinas = numMaquinas;
}

int Grafo::GetNumMaquinas() const {
    return numMaquinas;
}

void Grafo::SetNumTrabajos(int numTrabajos) {
    this->numTrabajos = numTrabajos;
}

int Grafo::GetNumTrabajos() const {
    return numTrabajos;
}

void Grafo::SetNombreFichero(string nombreFichero) {
    this->nombreFichero = nombreFichero;
}

string Grafo::GetNombreFichero() const {
    return nombreFichero;
}

vector<Nodo>& Grafo::GetVectorNodos()  {
    return vectorNodos;
}

void Grafo::visualizarGrafo(){
    
     
    int indice=0;
    for(int i=0;i<vectorNodos.size();i++){     
       
        cout<<"----------------NODO "<<indice<<"---------- Final: "<<vectorNodos[indice].isFinal()<<endl;
        cout<<"ID: "<<vectorNodos[indice].getID()<<endl;
        cout<<"Tiempo: "<<vectorNodos[indice].getTiempo()<<",Trabajo: "<<vectorNodos[indice].getNumTrabajo()<<",Maquina: "<<vectorNodos[indice].getNumMaquina()<<endl;
        cout<<"Tiempo Min: "<<vectorNodos[indice].getTiempoMinMaq()<<" Tiempo Max: "<<vectorNodos[indice].getTiempoMaxMaq()<<endl;
        
        
        if(vectorNodos[indice].getSiguiente()){
             cout<<"Sucesor directo: "<<vectorNodos[indice].getSiguiente()->getID()<<" Duracion: "<<vectorNodos[indice].getSiguiente()->getTiempo()<<endl;
        }
       
        
        
        cout<<"--Sucesores:--"<<endl;
        if(!vectorNodos[indice].getSucesores().empty()){
            for(int k=0;k<vectorNodos[indice].getSucesores().size();k++){
                cout<<"Nodo "<<vectorNodos[indice].getSucesores()[k]->getID()<<",Duracion: "<<vectorNodos[indice].getSucesores()[k]->getTiempo()<<endl;
            }

        }
        if(vectorNodos[indice].getSucesor()!=0)
            cout<<"Nodo Sucesor: "<<vectorNodos[indice].getSucesor()->getID()<<endl;

        indice++;                      
    }

}
 

Grafo& Grafo::operator =(const Grafo& orig){
    /*
     * Tienes que hacer una copia del vector
     * Una vez hecha,enlazo los punteros con la nueva estructura
     */
    this->nombreFichero=orig.nombreFichero;
    this->numMaquinas=orig.numMaquinas;
    this->numTrabajos=orig.numTrabajos;
    this->tiempoOptimo=orig.tiempoOptimo;
    this->vectorNodos=orig.vectorNodos;
    vectorNodos[0].vaciarSucesores();   
       //Ahora tengo que copiar los punteros
    for(int i=0;i<orig.vectorNodos.size();i++){
        
        //Copiar el vector de sucesores
        for(int j=0;j<orig.vectorNodos[i].getSucesores().size();j++){
            vectorNodos[i].anadirSucesor(&vectorNodos[orig.vectorNodos[i].getSucesores()[j]->getID()]);
        }
        
        //Copiar los punteros siguientes
        if(orig.vectorNodos[i].getSiguiente()!=0){
             vectorNodos[i].setSiguiente(&vectorNodos[orig.vectorNodos[i].getSiguiente()->getID()]); 
            
        }
        else
        {
            vectorNodos[i].setSiguiente(0);
        }
       
        //Copiar los punteros sucesores
        if(orig.vectorNodos[i].getSucesor()!=0){
            vectorNodos[i].setSucesor(&vectorNodos[orig.vectorNodos[i].getSucesor()->getID()]);            
        }
        else
        {
            vectorNodos[i].setSucesor(0);           
        }
        
    }
    
    
}

void Grafo::caminoCritico(Nodo* inicial, int cMaxActual, int& cMax, 
                          vector<list<Nodo*> >& recorrido, int& numCamino){
    
    
    //Primero compruebo si tiene sucesor directo
    recorrido[numCamino].push_back(inicial);

    if(inicial->getSiguiente()!=0){
        
        caminoCritico(inicial->getSiguiente(),cMaxActual+inicial->getTiempo(),cMax,recorrido,numCamino);
    }

     //Compruebo si tiene otros sucesores
    if(inicial->getSucesor()!=0){
       
        caminoCritico(inicial->getSucesor(),cMaxActual+inicial->getTiempo(),cMax,recorrido,numCamino);
    }

    if(inicial->isFinal())
    {   
        if(inicial->getTiempo()+cMaxActual>cMax){

            numCamino+=1;
           
            recorrido[numCamino]=recorrido[numCamino-1];
            recorrido[numCamino].pop_back();
            cMax=inicial->getTiempo()+cMaxActual;
            
        }
        else
        {
            recorrido[numCamino].pop_back();
        }
    }
    else
    {
        recorrido[numCamino].pop_back();
    }

}

Nodo* Grafo::getNodoID(int id){
    for(int i=0;i<vectorNodos.size();i++){
        if(vectorNodos[i].getID()==id)
            return &vectorNodos[i];
    }
        
}


list<Nodo*> Grafo::nodosCaminoCritico(Nodo* inicial){
    
    
    int valorMaxActual=0;
    int cMax=0;//Se guarda la distancia del camino critico mas largo
    int numCaminos=0;
    list<Nodo*> listaNodos;
    vector<list<Nodo*> > recorrido;
    recorrido.resize(1000);
    //Obtenemos el varios caminos criticos con diferente cMax
      
    caminoCritico(&vectorNodos[inicial->getID()],valorMaxActual,cMax,recorrido,numCaminos);
    
    //Posteriormente copiamos el mayor resultado en la lista que contendra los nodos del camino critico
    list<Nodo*>::iterator it;
    
    int valorCmaxAux=0;
    
    for(int i=0;i<recorrido.size();i++){
       
        it=recorrido[i].begin();
        while(it!=recorrido[i].end()){
            valorCmaxAux+=(*it)->getTiempo();
            it++;
        }
    
        if((valorCmaxAux==cMax)){
            listaNodos=recorrido[i];
            break;
        }
        valorCmaxAux=0;
    }
    
    return listaNodos;
    
    
}
        

long int Grafo::calcularCMAX(Nodo* inicial){
    
    long int contador=0;
    list<Nodo*>::iterator it;
    list<Nodo*> listaNodos=this->nodosCaminoCritico(inicial);
    
    
    it=listaNodos.begin();
    while(it!=listaNodos.end()){        
        contador+=(*it)->getTiempo();
        it++;
    }
    
    return contador;
    
    
}


void Grafo::restablecerGrafo(const vector<vector<Nodo*> >& matrizMaquinas){
    
    for(int i=0;i<matrizMaquinas.size();i++){
        for(int j=0;j<matrizMaquinas[i].size();j++){
            if(j==(int)(matrizMaquinas[i].size()-1)){
                 vectorNodos[matrizMaquinas[i][j]->getID()].setSucesor(0);
            }
            else
            {
                 vectorNodos[matrizMaquinas[i][j]->getID()].setSucesor(&vectorNodos[matrizMaquinas[i][j+1]->getID()]);
            }
           
        }
    }
    
    
}
