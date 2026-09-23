#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

pid_t pidAglobal;



void Zfunction(){
   printf("soy %d y acabo de recibir mi alarma\n",getpid());
   kill(pidAglobal,SIGUSR1);
}


void Afunction(){
   printf("soy %d y me ha avisado Z\n",getpid());
}






int main(int argc, char *argv[]){
    
    if(argc!= 2){
      exit(0);
    }
    
    int seconds=atoi(argv[1]);

    
    printf("Soy el proceso ejec: mi pid es %d\n", getpid());//esto solo lo imprime ejec
   
    pid_t pidEjec = getpid();

    pid_t pid = fork(); //el proceso A comienza su ejecución aqí
   
    if(pid == 0){
      pidAglobal = getpid();

      printf("Soy el proceso A: mi pid es %d. Mi padre es %d\n",getpid(), getppid());
      
      signal(SIGUSR1,Afunction);
      pid = fork(); // proceso B comienza aquí;
      
      if(pid == 0){
      printf("Soy el proceso B: mi pid es %d. Mi padre es %d. Mi abuelo es %d\n",getpid(), getppid(),pidEjec);
      }else{
        wait(NULL); //hacemos que A espere a que su hijo finalice
      }
    }else{ 

      wait(NULL);//hacemos que ejec espere a que su hijo directo finalice

    }
 
    //esto lo hace solo B
    pid =fork(); //crea a X

    if(pid ==0){
     printf("Soy el proceso X: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n",getpid(), getppid(),pidAglobal,pidEjec);
     pause();
    }
    pid = fork();

    if(pid ==0){
     printf("Soy el proceso Y: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n",getpid(), getppid(),pidAglobal,pidEjec);
     pause();
    }
    
    pid = fork();
    
    if(pid ==0){
     printf("Soy el proceso Z: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n",getpid(), getppid(),pidAglobal,pidEjec);

    }else{
      pause();
    }

    printf("solo deberia ser yo %d\n",getpid());
    signal(SIGALRM,Zfunction);
    alarm(seconds);
    pause();

   
  
    exit(0);


}





