#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "virtual_processor.h"
#define FEEDBACK_QUEUES 5

extern struct PROCESO proceso[];
extern struct COLAPROC listos, bloqueados;
extern int tiempo;
extern int pars[];

struct COLAPROC feedback_queue[FEEDBACK_QUEUES];
bool initialize_feedback_queues = true;
int time_left = 1;

void put_in_feedback_queue(int num_proceso);


int scheduler(int evento) {
  bool cambia_proceso = false;
  int prox_proceso_a_ejecutar = pars[1];

  if (initialize_feedback_queues) {
    for (int i = 0; i < FEEDBACK_QUEUES; i++) {
      feedback_queue[i].ent = 0;
      feedback_queue[i].sal = 0;
    }
    initialize_feedback_queues = false;
  }

  if (evento == PROCESO_NUEVO) {
    printf("Proceso nuevo: %d\n", pars[0]);
    proceso[pars[0]].estado = LISTO;
    put_in_feedback_queue(pars[0]);
    cambia_proceso = tiempo == 0;
  }

  if (evento == TIMER) {
    if (pars[1] == NINGUNO) {
      cambia_proceso = true;
    } else if (proceso[pars[1]].trestante == 0) {
      printf("Interrupcion del proceso por quantum\n");
      proceso[pars[1]].prioridad++;
      proceso[pars[1]].estado = LISTO;
      put_in_feedback_queue(pars[1]);
      cambia_proceso = true;
    } else {
      proceso[pars[1]].trestante--;
      printf("Proceso %d continua con q=%d\n", pars[1], proceso[pars[1]].trestante);
    }
  }

  if (evento == SOLICITA_E_S) {
    printf("Solicita E/S Proceso %d\n", pars[1]);
    proceso[pars[1]].estado = BLOQUEADO;
    cambia_proceso = true;
  }

  if (evento == TERMINA_E_S) {
    printf("Termina E/S Proceso desbloqueado %d\n", pars[0]);
    proceso[pars[0]].estado = LISTO;
    put_in_feedback_queue(pars[0]);
  }

  if (evento == PROCESO_TERMINADO) {
    printf("Termina el proceso %d\n", pars[0]);
    proceso[pars[0]].estado = TERMINADO;
  }

  if (cambia_proceso) {
    prox_proceso_a_ejecutar = NINGUNO;
    for (int i = 0; i < FEEDBACK_QUEUES; i++) {
      if (!cola_vacia(feedback_queue[i])) {
        prox_proceso_a_ejecutar = sacar_de_cola(&feedback_queue[i]);
        proceso[prox_proceso_a_ejecutar].estado = EJECUCION;
        proceso[prox_proceso_a_ejecutar].trestante = 1;
        printf("Siguiente a ejcutar: %d (de la cola %d, q=%d)\n", prox_proceso_a_ejecutar, i, time_left);
        break;
      }
    }
  }

  return prox_proceso_a_ejecutar;
}

void put_in_feedback_queue(int num_proceso) {
  if (proceso[num_proceso].prioridad >= FEEDBACK_QUEUES) {
    proceso[num_proceso].prioridad = FEEDBACK_QUEUES - 1;
  }
  mete_a_cola(&feedback_queue[proceso[num_proceso].prioridad], num_proceso);
  printf("Proceso %d entra en la cola de prioridad %d\n",
    num_proceso, proceso[num_proceso].prioridad);
}
