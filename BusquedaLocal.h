/* 
 * File:   BusquedaLocal.h
 * Author: Drebin
 *
 * Created on 29 de noviembre de 2015, 12:25
 */

#ifndef BUSQUEDALOCAL_H
#define	BUSQUEDALOCAL_H
#include "Maquina.h"
#include "Grafo.h"
#include "Nodo.h"
#include <queue>


class BusquedaLocal {
private:
    
    vector<vector<Nodo*> > generarBloquesCriticos(list<Nodo*> &nodosCaminoCritico,int numeroMaquinas);
    
public:
    BusquedaLocal();
    virtual ~BusquedaLocal();
    
    /*
     * Aplicando busqueda local, devuelve el cmax obtenido
     */
    int aplicarBusquedaLocal(Maquina *maquina,Grafo *grafo);
    
};

#endif	/* BUSQUEDALOCAL_H */

