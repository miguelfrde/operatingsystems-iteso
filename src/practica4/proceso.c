#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

int numproc;

int main(int argc, char *argv[])
{
    long long start_ts;
    long long stop_ts;
    long long current_ts;
    long long elapsed_time;
    long lElapsedTime;
    struct timeval ts,ti,tf;
    int i,j;
    int tes;
    long long tcpu;
	
    numproc=atoi(argv[1]);	  
   
    tcpu=atoi(argv[3]);

    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec * 1000000 + ts.tv_usec;

    // Uso del CPU	
    // for(i=0;i<atoi(argv[2]);i++)    
    //    for(j=0;j<atoi(argv[3]);j++);
  
    for(i=0;i<tcpu;i++)
    {
        for(j=0;j<100;j++)
        {
            gettimeofday(&ti,NULL);
            do 
            {
                gettimeofday(&tf, NULL);
                current_ts=(tf.tv_sec*1000000+tf.tv_usec)-(ti.tv_sec*1000000+ti.tv_usec);
            } while(current_ts<10000);
        }
    }

 
    tes=atoi(argv[4]);
    if(tes!=0)
    { 
        iocall(tes);
    }
    
    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec * 1000000 + ts.tv_usec;
	 
    elapsed_time = stop_ts - start_ts;
    printf("duración proceso %d, %d microsegundos\n",atoi(argv[1]),(int) elapsed_time);
   exit(0);    
}

int iocall(int tes)
{
    int pidkernel,statval;
    FILE *fp;

    pidkernel=getppid();
    // Llama a entrada y salida
    // usleep(500000);
    kill(getppid(),SIGUSR1);
    sleep(tes);
    fp=fopen("fifo0","w");
    if(fp==NULL)
    {
        perror("Error al abrir el pipe0");
        exit(1);
    }
    fprintf(fp,"%d\n",numproc);
    fflush(fp);
    fclose(fp);
    kill(getppid(),SIGUSR2);
    raise(SIGSTOP);
    return(0);
}
