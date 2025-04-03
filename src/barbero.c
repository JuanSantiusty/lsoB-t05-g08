#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "barbero.h"

// Variables globales
int sillasDisponibles = 0;
int sillasTotales = 0;
int clientesTotales = 0;
int clienteEsperando = 0;
int barberoOcupado = 0;

// Variables para sincronización
pthread_mutex_t mutexBarbero = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condBarbero = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutexCliente = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condCliente = PTHREAD_COND_INITIALIZER;

// Función para simular el corte de cabello
void cortar_cabello() {
    printf("Barbero está cortando el cabello...\n");
    sleep(2);  // Simula el tiempo de corte
    printf("Cabello cortado.\n");
}

// Función para clientes que se van
void salir(int numCli) {
    printf("El cliente %d se fue de la barbería porque no había sillas disponibles.\n", numCli);
}

// Función para el hilo del barbero
void* barbero(void* arg) {
    while (clientesTotales > 0) {
        pthread_mutex_lock(&mutexCliente);
        while (clienteEsperando == 0) {
            printf("Barbero está durmiendo...\n");
            pthread_cond_wait(&condCliente, &mutexCliente);
        }
        clienteEsperando--;
        barberoOcupado = 1;
        pthread_mutex_unlock(&mutexCliente);

        // Cortar el cabello del cliente
        if(clientesTotales > 0){
        cortar_cabello();
        }

        // Libera la silla
        pthread_mutex_lock(&mutexBarbero);
        sillasDisponibles++;
        barberoOcupado = 0;
        pthread_cond_signal(&condBarbero);
        pthread_mutex_unlock(&mutexBarbero);
    }
}

// Función para los hilos de los clientes
void* cliente(void* numCliente) {
    int num = *(int*)numCliente;

    pthread_mutex_lock(&mutexBarbero);
    if (sillasDisponibles > 0) {
        printf("Cliente %d se sentó en una silla de espera.\n", num);
        sillasDisponibles--;

        clienteEsperando++;
        pthread_cond_signal(&condCliente);
        
         // Esperar al barbero
        while (barberoOcupado) {
            pthread_cond_wait(&condBarbero, &mutexBarbero);
        }
        
        clientesTotales--;
        pthread_mutex_unlock(&mutexBarbero);
    } else {
        salir(num);
        clientesTotales--;
        pthread_mutex_unlock(&mutexBarbero);
    }

    
}

// Función principal para ejecutar el problema del barbero
void ejecutar_barbero(int numSillas, int numClientes) {
    sillasDisponibles = numSillas;
    sillasTotales = numSillas;
    clientesTotales = numClientes;

    pthread_t hiloBarbero;
    pthread_t hilosClientes[numClientes];

    // Crear hilo del barbero
    pthread_create(&hiloBarbero, NULL, barbero, NULL);

    // Crear hilos de clientes
    for (int i = 0; i < numClientes; i++) {
        int* idCliente = malloc(sizeof(int));
        *idCliente = i + 1;  // Números de clientes desde 1
        sleep(1);
        pthread_create(&hilosClientes[i], NULL, cliente, idCliente);
    }

    // Esperar que terminen los hilos
    pthread_join(hiloBarbero, NULL);
    for (int i = 0; i < numClientes; i++) {
        pthread_join(hilosClientes[i], NULL);
    }

    // Liberar recursos
    pthread_mutex_destroy(&mutexCliente);
    pthread_cond_destroy(&condCliente);
    pthread_mutex_destroy(&mutexBarbero);
    pthread_cond_destroy(&condBarbero);
}


