#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include "virtual_processor.h"
#define VERSION "Virtual Processor versión: 6.0913.07\n"

struct COLAPROC listos,bloqueados;

void proc(int pid);
void handler();
long get_one_millisec_loop();
void mete_a_cola(struct COLAPROC *q,int proceso);

struct PROCESO proceso[MAXPROC];


int tiempo=-1;
int tiempo_total=0;
int pars[2];
int proceso_en_ejecucion;
int proceso_terminado;
int cambia_proceso=1;
FILE *fp;


int main(int argc,char *argv[]) 
{
    int res,p,pid,i,thrid,statval;
    char snumproc[5],soml[12],stcpu[5],stes[5];
   
    struct itimerval itimer, otimer;
    struct sigaction act;

    if(argc>2)
    {
        fprintf(stderr,"Error en los argumentos\nUso: process [/version]\n");
        exit(1);
    }
    if(argc==2)
        if (strcmp(argv[1],"/version")==0)
        {
            printf(VERSION);
            exit(0);
        }
        else
        {
            fprintf(stderr,"Error en los argumentos\nUso: process [/debug]\n");
            exit(1);
        }

 
    // Obtener los ciclos por tiempo 
    sprintf(soml,"%d",(int) get_one_millisec_loop());   
    printf("%s\n",soml); 

    // Crear semáforo para asegurarse que solo 1 proceso pueda ejecutar handler
    // non_reentrant=semget((key_t)5234, 1, 0666 | IPC_CREAT);
    // set_semvalue(1,non_reentrant);

    // Crear pipe para que el scheduller sepa cual proceso terminó E/S
    unlink("fifo0");
    mkfifo("fifo0",0777);
    fp=fopen("fifo0","r+");

    // Pedir los datos de entrada al usuario
    for(p=0;p<MAXPROC;p++)
    {
         printf("Proceso %d\n",p);
         printf("   Tiempo de llegada ....................... : ");
         scanf("%d",&proceso[p].tinicio);
         printf("   Tiempo de cpu ........................... : ");
         scanf("%d",&proceso[p].tcpu);
         printf("   Tiempo de entrada y salida .............. : ");
         scanf("%d",&proceso[p].tes);
         printf("   Prioridad (usar solo para fixed priority) : ");
         scanf("%d",&proceso[p].prioridad);

         tiempo_total+=proceso[p].tcpu;
         proceso[p].trestante=proceso[p].tcpu;
         proceso[p].estado=NUEVO;
    }


    // Crear los procesos
    for(p=0;p<MAXPROC;p++)
    {
        proceso[p].pid=fork();
        if(proceso[p].pid==0)
        {
            kill(getpid(),SIGSTOP);
            sprintf(snumproc,"%d",p);
            sprintf(stcpu,"%d",proceso[p].tcpu);
            sprintf(stes,"%d",proceso[p].tes);
            execlp("./proceso","proceso",snumproc,soml,stcpu,stes,NULL);
        }
    }
    
    printf("Proceso %d\n",getpid());

    // Establece el handler para manejar las señales

    act.sa_handler=handler;
    sigaddset(&act.sa_mask,SIGALRM);
    sigaddset(&act.sa_mask,SIGUSR1);
    sigaddset(&act.sa_mask,SIGUSR2);
    act.sa_flags=SA_RESTART;
    sigaction(SIGALRM,&act,0);

    act.sa_handler=handler;
    sigaddset(&act.sa_mask,SIGALRM);
    sigaddset(&act.sa_mask,SIGUSR1);
    sigaddset(&act.sa_mask,SIGUSR2);
    act.sa_flags=SA_RESTART;
    sigaction(SIGUSR1,&act,0);

    act.sa_handler=handler;
    sigaddset(&act.sa_mask,SIGALRM);
    sigaddset(&act.sa_mask,SIGUSR1);
    sigaddset(&act.sa_mask,SIGUSR2);
    act.sa_flags=SA_RESTART;
    sigaction(SIGUSR2,&act,0);


    // Inicializa variabless
    listos.ent=0;
    listos.sal=0;
    bloqueados.ent=0;
    bloqueados.sal=0;
    proceso_en_ejecucion=NINGUNO;

    // Inicia el timer
    itimer.it_interval.tv_sec=1;
    itimer.it_interval.tv_usec=0;
    itimer.it_value.tv_sec=1;
    itimer.it_value.tv_usec=0;

    if(setitimer(ITIMER_REAL,&itimer,&otimer)<0)
         perror("Error en el settimer");
    

    // Aquí vamos a esperar a los procesos
    //
    //
    for(p=0;p<MAXPROC;p++)
    {
        pid=wait(&statval);
	for(i=0;i<MAXPROC;i++)
	    if(proceso[i].pid==pid)
            {
                proceso_terminado=i;
                handler(SIGCHLD);
	    }
    }
    // del_semvalue(non_reentrant);   
    fclose(fp);

    exit(EXIT_SUCCESS);
}

	    
void handler(int sen)
{
    int evento,ejecuta;
    char line_buffer[4];

    // Checar si hay procesos nuevos y si los hay meterlos a la cola
    if(sen==SIGALRM)
    {
        tiempo++;
	printf("\n--------------------------\nInterrupcion del timer proceso\nTiempo=%d\n",tiempo);

        for(pars[0]=0;pars[0]<MAXPROC;pars[0]++)
            if(proceso[pars[0]].tinicio==tiempo)
            {
                evento=PROCESO_NUEVO;
                printf("Proceso Nuevo %d\n",pars[0]);
                if(proceso_en_ejecucion!=NINGUNO && proceso[proceso_en_ejecucion].estado!=BLOQUEADO)
                    kill(proceso[proceso_en_ejecucion].pid,SIGSTOP); 
                pars[1]=proceso_en_ejecucion;
                ejecuta=scheduler(evento);
                printf("<---PLANIFICADOR--->\n");
                proceso_en_ejecucion=ejecuta;
                if(proceso_en_ejecucion!=NINGUNO && proceso[proceso_en_ejecucion].estado!=BLOQUEADO)
                {
                    printf("Proceso en turno %d\n",ejecuta);
                    kill(proceso[proceso_en_ejecucion].pid,SIGCONT);
                }
            }

            if(proceso_en_ejecucion!=NINGUNO  && proceso[proceso_en_ejecucion].estado!=BLOQUEADO)
            {
                printf("Llegada del timer, se supende proceso %d\n",proceso_en_ejecucion);
                kill(proceso[proceso_en_ejecucion].pid,SIGSTOP);
            }
            pars[0]=proceso_en_ejecucion;
            evento=TIMER;
    }

    if(sen==SIGUSR1)
    {
        printf("\n--------------------------\nProceso %d solicita E/S\n",proceso_en_ejecucion);
        evento=SOLICITA_E_S;
        pars[0]=proceso_en_ejecucion;
    }

    if(sen==SIGUSR2)
    {
        evento=TERMINA_E_S;
        // Saber cual proceso terminó E/S
        fgets(line_buffer,4,fp);
        pars[0]=atoi(line_buffer);

        printf("\n--------------------------\nTerminación de entrada y salida del proceso %d\n",pars[0]);
        if(proceso_en_ejecucion!=NINGUNO && proceso[proceso_en_ejecucion].estado!=BLOQUEADO)
        {
            printf("Se suspende el proceso %d\n",proceso_en_ejecucion);
            kill(proceso[proceso_en_ejecucion].pid,SIGSTOP);
        }

    }

    if(sen==SIGCHLD)
    {
        printf("\n--------------------------\nTermina ejecución del proceso %d\n",proceso_terminado);
        evento=PROCESO_TERMINADO;
        proceso_en_ejecucion=NINGUNO;
        pars[0]=proceso_terminado;

    }
    pars[1]=proceso_en_ejecucion;
    ejecuta=scheduler(evento); // Llama al scheduler
    printf("<---PLANIFICADOR--->\n");
    proceso_en_ejecucion = ejecuta; 
    if(ejecuta!=NINGUNO  && proceso[ejecuta].estado!=BLOQUEADO)
    {
        printf("Proceso en ejecucion %d\n",ejecuta);
        kill(proceso[ejecuta].pid,SIGCONT);
    }
   
    return;
}

long get_one_millisec_loop()
{
    long long start_ts;
    long long stop_ts;
    long long elapsed_time;
    long lElapsedTime;
    long cicles;
    struct timeval ts;
    int i,j;
 
 
    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec * 1000000 + ts.tv_usec;
 
 
    for (i=0; i< 1000000 ; i++);
 
    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec * 1000000 + ts.tv_usec;
 
    elapsed_time = (stop_ts - start_ts)/1000;
 
    cicles=10000000/elapsed_time;
    return(cicles);
}



void mete_a_cola(struct COLAPROC *q,int proceso)
{
    q->cola[q->ent]=proceso;
    q->ent++;
    if(q->ent>19)
       q->ent=0;
}

int cola_vacia(struct COLAPROC q)
{
    if(q.ent==q.sal)
        return(1);
    else
        return(0);
}

int sacar_de_cola(struct COLAPROC *q)
{
    int proceso;
    
    proceso=q->cola[q->sal];
    q->sal++;
    if(q->sal>19)
        q->sal=0;
    return(proceso);
} 

int num_en_tabla(int pid)
{
    int i;
    for(i=0;i<MAXPROC;i++)
        if(proceso[i].pid==pid)
            break;
    return(i);
}
