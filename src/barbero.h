#ifndef BARBERO_H
#define BARBERO_H

#include <pthread.h>

//Funciones para barbero y cliente
void* barbero(void* arg);
void* cliente(void* numCliente);
void cortar_cabello();
void salir(int numCli);

// Ejecutar problema del barbero
void ejecutar_barbero(int numSillas,int numClientes);


#endif
