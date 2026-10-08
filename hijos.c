#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>



void parse_args(char *argv[],int *x,int *y){
*x = atoi(argv[1]);
*y = atoi(argv[2]);
}



void create_X(int x, pid_t x_pids[x],pid_t* lastX){

    for(int i=0; i<x; i++){
        x_pids[i] = getpid();

        if(fork() >0){
            break;
        }
        if(i == x-1){
            *lastX = getpid();
        }
    }


}




int main(int argc, char *argv[]){

    
    int x,y;
    if(argc != 3){
        printf("ERROR: Argumentos inválidos\n");
        exit(0);
    }
    
    parse_args(argv,&x,&y); //inicializa x e y


    pid_t x_pids[x]; //aray de padres x
    pid_t lastX=0; //pid del último x para crear a los hijos  yc
    pid_t father = getpid(); //pid del superpadre(para imprimir los hijos finales)
    

    //creación del segmento de memoria compartida
    
    int shmid = shmget(IPC_PRIVATE,sizeof(pid_t) *y, IPC_CREAT|0666);
    if(shmid <0){
        printf("error creando segmento de memoria compartida\n");
        exit(1);
    }
    pid_t *y_pids = (pid_t *) shmat(shmid,NULL,0);

    if(y_pids == (pid_t*)(-1)){
        printf("error acoplando el segmento de memoria compartida\n");
        exit(1);
    }

    if(fork() ==0){
       create_X(x, x_pids, &lastX); //creamos el primer hijo de x y hacemos que cree al resto para que el array solo contenga los padres x
    }

    sleep(20);

    exit(0);
}