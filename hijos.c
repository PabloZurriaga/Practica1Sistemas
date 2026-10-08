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

         if(i == x-1){
            *lastX = getpid();
            break;
        }

        if(fork() >0){
            break;
        }
       
    }


}

void print_and_exit(int x, int x_pids[x]){
    pause();//no imprimen mensaje hasta que el padre se lo indique
    printf("Soy el subhijo %d, mi padres son:",getpid());
    for(int i=0; i < x; i++){
            if(i != x-1){
               printf("%d,",x_pids[i]);
            }else{
               printf("%d\n",x_pids[i]);
            }
            
    }
    exit(0);
}



void continue_father(){

}
void continue_y(){

}
void create_Y(int y, pid_t * y_pids, int x, int x_pids[x],pid_t father){
    for(int i=0; i < y; i++){

        pid_t pid = fork();

        if(pid ==0){
            signal(SIGUSR2,continue_y); //los hijos continuarán cuando el padre selo indique
            shmdt(y_pids); //desacoplamos el segmento de memoria de la fila antes de que hagan exit
            print_and_exit(x,x_pids);
            
        }else if(pid > 0){
            y_pids[i] = pid;
        }
    }
    kill(father,SIGUSR1); // avisa al padre para que impima el mensaje cuando los hijos han insertado sus pids en el segmento de memoria compartida

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
    signal(SIGUSR1,continue_father);
    signal(SIGUSR2,SIG_IGN);

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

    if(getpid() == lastX){
        create_Y(y, y_pids,x,x_pids,father);
    }
    
    if(getpid() == father){
        
        pause();//esperamos a que los hijos finales introduzcan su pid en el segmento de memoria
        printf("Soy el superpadre (%d) : mis hijos finales son: ",getpid());
        for(int i =0; i < y; i++){
           if(i != y-1){
              printf("%d,",y_pids[i]);
           }else{
              printf("%d\n",y_pids[i]);
           }
        }
        kill(0,SIGUSR2);
        wait(NULL);
        shmdt(y_pids);
        shmctl(shmid,IPC_RMID,NULL);
        exit(0);
        
    }

    if(getpid() == lastX){
        for(int i =0; i < y-1; i++){
            wait(NULL);
        }
    }

     shmdt(y_pids);
     wait(NULL); //pausamos a la fila x
     exit(0);
}