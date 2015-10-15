#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include "virtual_processor.h"

extern struct PROCESO proceso[];
extern struct COLAPROC listos, bloqueados;
extern int tiempo;
extern int pars[];

int queue_size(struct COLAPROC q);

int scheduler(int evento) {

  bool cambia_proceso = false;
  int prox_proceso_a_ejecutar = pars[1];

  if (evento == PROCESO_NUEVO) {
    proceso[pars[0]].estado = LISTO;
    mete_a_cola(&listos, pars[0]);
    cambia_proceso = true;
  }

  if (evento == TIMER) {
    printf("Llega interrupcion del Timer\n");
    proceso[pars[0]].estado = LISTO;
    cambia_proceso = true;
  }

  if (evento == SOLICITA_E_S) {
    proceso[pars[1]].estado = BLOQUEADO;
    printf("Solicita E/S Proceso %d\n", pars[1]);
    cambia_proceso = true;
  }

  if (evento == TERMINA_E_S) {
    proceso[pars[0]].estado = LISTO;
    cambia_proceso = true;
    printf("Termina E/S Proceso desbloqueado %d\n", pars[0]);
  }

  if (evento == PROCESO_TERMINADO) {
    proceso[pars[0]].estado = TERMINADO;
    cambia_proceso = true;
  }

  if (cambia_proceso) {
    if (!cola_vacia(listos)){
    	int tmp_proc;
    	int min = INT_MAX;
    	prox_proceso_a_ejecutar = NINGUNO;
    	for(int i=0; i<queue_size(listos); i++){
    		tmp_proc = sacar_de_cola(&listos);
    		if(proceso[tmp_proc].estado == TERMINADO){
					continue;
    		}

    		if(min >= proceso[tmp_proc].trestante){
    			min = proceso[tmp_proc].trestante;
    			prox_proceso_a_ejecutar = tmp_proc;
    		}
    		mete_a_cola(&listos, tmp_proc);

    	}
      proceso[prox_proceso_a_ejecutar].estado = EJECUCION;
      cambia_proceso = false;
    }
    if (prox_proceso_a_ejecutar == NINGUNO){
    	printf("Ho hay procesos en cola\n");
    }
  }
  return prox_proceso_a_ejecutar;
}

int queue_size(struct COLAPROC q){
	return q.size;
}

