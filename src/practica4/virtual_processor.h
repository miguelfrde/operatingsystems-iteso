#define MAXPROC 3 

#define NINGUNO -1

#define NUEVO 0
#define LISTO 1 
#define EJECUCION 2
#define BLOQUEADO 3
#define TERMINADO 4 

#define TIMER 0
#define SOLICITA_E_S 1
#define TERMINA_E_S 2
#define PROCESO_NUEVO 3

#define PROCESO_TERMINADO 4

struct PROCESO {
    int pid;
    int tinicio;
    int tcpu;
    int tes;
    int trestante;
    int prioridad;
    int estado;
};

struct COLAPROC {
    int cola[20];
    int ent;
    int sal;
};

void mete_a_cola(struct COLAPROC *q,int proceso);
int cola_vacia(struct COLAPROC q);
int sacar_de_cola(struct COLAPROC *q);
