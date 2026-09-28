//PABLO ZURRIAGA GONZÁLEZ 54204705F
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>




void parse_args(char *argv[], int *x, int *y){
         *x = atoi(argv[1]);
         *y = atoi(argv[2]);
}


void create_rows(int x){  //creamos el número de filas determinado por y
 for(int i =0; i < x -1; i++){
    if(fork()>0){
        return; // todos menos la última fila volverá aquí
    }
 }

pause(); //la última fila no es inmunne a la señal SIGUSR1

}




void create_columns(int y){ //creamos el número de columnas determinado por x
for(int i=0; i <y; i++){
    if(fork() ==0)
        return;
    }
}

void malla_handler(int sig){
if(fork() ==0){
   char pid_str[16];
    sprintf(pid_str, "%d", getppid()); // Obtiene el pid de malla y lo pasa a texto
        execlp("pstree", "pstree", "-c", pid_str, NULL);
     exit(0);
}else{
    wait(NULL); //espera a que pstree muera y envia la señal a todos, peor solo muere la última fila de y
    signal(SIGUSR1,SIG_IGN); //hacemos a malla inmune a la señal que va a enviar;
    kill(0,SIGUSR1);
    
}
}


int main(int argc,char *argv[]){

    if(argc != 3){
         printf("ERROR: número de argumentos erróneo\n"); //comprobamos que hay dos argumentos(x e y) además de ./malla
        exit(0);
    }
    //comprobamos que el número de argumentos sea correcto e inicializamos x e y, que serán las filas y columnas
    int x,y;

    parse_args(argv,&x,&y);

    pid_t pid_malla= getpid();

    create_columns(y);

    if(getpid() == pid_malla){
        signal(SIGALRM,malla_handler);
        alarm(2);
        pause();

        for(int i=0; i < y; i++){
            wait(NULL); //malla debe hacer wait tantas veces como hijos DIRECTOS tenga
        }
    
        exit(0);
    }else{
         create_rows(x); 
         //aquí están todos menos malla y la última fila de la matriz
         signal(SIGUSR1,SIG_IGN); // los que no sean la última fila deben morir de forma secuencial, no les debe afectar el primer kill
         wait(NULL);
         exit(0);
    }    
  
 
    exit(0);
}