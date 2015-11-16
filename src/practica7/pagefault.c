#include <stdio.h>
#include "mmu.h"

#define RESIDENTSETSIZE 24

extern char *base;
extern int idproc;
extern int systemframetablesize;
extern int processpagetablesize;

extern struct SYSTEMFRAMETABLE *systemframetable;
extern struct PROCESSPAGETABLE processpagetable[];


// Rutina de fallos de página

int pagefault(char *vaddress)
{
    int i;
    int frame;
    int pag_del_proceso;

    // Calcula la página del proceso
    pag_del_proceso=(int) vaddress>>12;
    // Cuenta los marcos asignados al proceso
    i=countframesassigned();

    // Busca un marco libre en el sistema
    frame=getfreeframe();

    if(frame==-1)
    {
        return(-1); // Regresar indicando error de memoria insuficiente
    }


    processpagetable[pag_del_proceso].presente=1;
    processpagetable[pag_del_proceso].framenumber=frame;


    return(1); // Regresar todo bien
}


int getfreeframe()
{
    int i;
    // Busca un marco libre en el sistema
    for(i=0;i<systemframetablesize;i++)
        if(!systemframetable[i].assigned)
        {
            systemframetable[i].assigned=1;
            break;
        }
    if(i<systemframetablesize)
        systemframetable[i].assigned=1;
    else
        i=-1;
    return(i);
}

