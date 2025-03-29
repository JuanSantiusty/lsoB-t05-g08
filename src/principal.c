#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "barbero.h"

//Variables para guardar el numero de sillas y clientes
int  NUM_SILLAS=0;
int  NUM_CLIENTES=0;


/**
 *@brief Funcion main que recibe por consola el numero de sillas y de clientes
 *@param argc Numero de parametros iniciales que recibe
 *@param Arreglo de texto donde de guardaran el nuemro de sillas y clientes
 *@return 0 Si todo sale bien retorna 0
 */
int main (int argc,char *argv[]){
  //Verificar si los parametros requeridos fueron ingresados
  if(argc!=3){
    printf("Usar ./taller5 <NUM_SILLAS> <NUM_CLIENTES>");
    exit(-1);
  }
  //Guardar el nuemro de sillas y clientes
  NUM_SILLAS=atoi(argv[1]);
  NUM_CLIENTES=atoi(argv[2]);
  //Ejecutar problema del barbero
  ejecutar_barbero(NUM_SILLAS,NUM_CLIENTES);
  return 0;
}
