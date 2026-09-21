#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>




int main(int argc, char argv[]){

    printf("Soy el proceso ejec: mi pid es %d", getpid());//esto solo lo imprime ejec

    pid_t pid = fork(); //el proceso A comienza su ejecución aqí
   
    if(pid == 0){
      printf("Soy el proceso A: mi pid es %d. Mi padre es %d\n",getpid(), getppid());
        pid = fork(); // proceso B comienza aquí;
    }
    



}