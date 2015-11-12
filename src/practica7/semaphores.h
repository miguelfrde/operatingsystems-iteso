#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

extern int errno;

#define EXIT_FAILURE 1


#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
    /* union semun is defined by including <sys/sem.h> */
#else
    /* according to X/OPEN we have to define it ourselves */
    union semun {
        int val;                    /* value for SETVAL */
        struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
        unsigned short int *array;  /* array for GETALL, SETALL */
        struct seminfo *__buf;      /* buffer for IPC_INFO */
    };
#endif



/* La función set_semvalue inicializa el semáforo usando el comando SETVAL
   en una llamada semctl. Es lo primero que hay que hacer para usar el 
   semáforo */

static void set_semvalue(int valinic,int idsem)
{
    union semun sem_union;

    sem_union.val = valinic;
    if (semctl(idsem, 0, SETVAL, sem_union) == -1) 
    {
        fprintf(stderr,"set_semvalue failed\n");
        exit(EXIT_FAILURE);
    }
}


/* La función del_semvalue tiene casi la misma forma, excepto que la llamada 
  semctl usa IPC_RMID para remover el ID del semáforo */

static void del_semvalue(int idsem)
{
    union semun sem_union;
    
    if (semctl(idsem, 0, IPC_RMID, sem_union) == -1)
        fprintf(stderr, "Failed to delete semaphore\n");
}

/* semaphore_p cambia el semáforo a -1 (esperando). */
static void semaphore_wait(int idsem)
{
    struct sembuf sem_b;
    
    sem_b.sem_num = 0;
    sem_b.sem_op = -1; /* P() */
    sem_b.sem_flg = SEM_UNDO;
    // while(semop(idsem, &sem_b, 1) == -1 && errno==EINTR);
    if (semop(idsem, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_wait failed\n");
        perror("Error es");
        exit(EXIT_FAILURE);
    } 
}


/* semaore_v es similar excepto que establece la parte de sem_op de la estructura sembuf a 1 */

static void semaphore_signal(int idsem)
{
    struct sembuf sem_b;
    
    sem_b.sem_num = 0;
    sem_b.sem_op = 1; /* V() */
    sem_b.sem_flg = SEM_UNDO;
    while(semop(idsem, &sem_b, 1) == -1 && errno==EINTR);
    if (semop(idsem, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_signal failed\n");
        exit(EXIT_FAILURE);
    }
}

