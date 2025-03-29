#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "barbero.h"

//Varible para saber el numero de sillas disponibles
int sillasDisponibles=0;
int sillasTotales=0;
int clientesTotales=0;
int cliente=0;
int barbero=0;
//Varibles para sincronizacion
pthread_mutex_t barbero = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t esperarBarbero =  PTHREAD_COND_INITIALIZER;

pthread_mutex_t cliente = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t esperarCliente =  PTHREAD_COND_INITIALIZER;


/**
*@brief Funcion para el hilo del barbero
*/
void* barbero(void* arg){
  //Mientras aun existan clientes esperando
  while(clientesTotales>0){
    //Esperar un cliente
    pthread_mutex_lock(&cliente);
    barbero=1;
    while(cliente==0){
      pthread_cond_wait(&esperarCliente,&cliente);
    }
    pthread_mutex_unlock(&cliente);
    //Cortar Cabello al cliente
    clientesEsperando-=1;
    cliente=0;
    cortar_cabello();
    if(sillasDisponibles<sillasTotales){
      sillasDisponibles+=1;
    }
  }
}

//Fyncion para los hilos de clientes
void* cliente(void* numCliente){
  int num=*(int *)numCliente;
  //Esperar barbero, pero si no ahi sillas disponibles abandonar el lugar
  pthread_mutex_lock(&barbero);
  //Espera 1 segundo por una silla
  sleep(1);
  if(sillasDisponible>0 && sillasDisponibles<=sillasTotales){
    sillasDisponibles-=1;
  }else{
    salir(num);
    clientesTotales-=1;
    pthread_mutex_unlock(&barbero);
  }
}

//Funcion para cortar cabello
void cortar_cabello(){
  printf("Cortando cabello");
  sleep(1);
  printf("Cabello cortado");
}

//Funcion para irse
void salir(int numCli){
  printf("El cliente %d salio de la barberia",&numCli);
}
/**
 *@brief Funcion que ejecuta el problema del barbero creando los hilos de brabero y clientes
 *@param numSillas Numero de sillas de la tienda
 *@param numClientes Numero de clientes que recibira la tienda
 */
void ejecutar_barbero(int numSillas,int numClientes){
  int i=0;
  sillasDisponibles=numSillas;
  sillasTotales=numSillas;
  clientesTotales=numClientes;
  //Declarar hilos de barbero y clientes
  pthread_t hiloBarbero;
  pthread_t hilosClientes[numClientes];

  //Crear hilo barbero
  pthread_create(&hiloBarbero,NULL,barbero,NULL);
  //Crear hilos de clientes
  for (i=0;i<numClientes;i++){
    pthread_create(&hilosClientes[i],NULL,cliente,(void*)i);
  }

  //Esperar que terminen los hilos
  pthread_join(hiloBarbero,NULL)
  for (i=0;i<numClientes;i++){
    pthread_join(hilosClientes[i],NULL);
  }
  //Liberar los mutex y condicionales
  pthread_mutex_destroy(&cliente);
  pthread_cond_destroy(&esperarCliente);
  pthread_mutex_destroy(&barbero);
  pthread_cond_destroy(&esperarBarbero);
}
