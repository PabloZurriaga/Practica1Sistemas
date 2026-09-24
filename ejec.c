#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

pid_t pidAglobal;
pid_t pidZglobal; //para la destruccion secuencial del arbol;

void Zfunction(){
  //envia señal  a A para que ejecute pstree
   kill(pidAglobal,SIGUSR1);
}


void Afunction(){ //A recibe la señal, crea un hijo que hace el pstree, espera a que este muera y luego hara un wait(NULL) en el main
   pid_t p =fork();
   if(p == 0){
    execlp("pstree", "pstree", NULL);
    exit(0); 
   }
  kill(pidZglobal,)
  wait(NULL);
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
        pause(); //hacemos que A espere a recibir la señal y luego a  que su hijo finalice
        wait(NULL);
        exit(0);
      }
    }else{ 
      wait(NULL);//hacemos que ejec espere a que su hijo directo finalice
      exit(0);
    }
 
    //esto lo hace solo B
    pid =fork(); //crea a X

    if(pid ==0){
     printf("Soy el proceso X: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n",getpid(), getppid(),pidAglobal,pidEjec);
     pause(); //paramos a X
     exit(0);
    }
    pid = fork();

    if(pid ==0){
     printf("Soy el proceso Y: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n",getpid(), getppid(),pidAglobal,pidEjec);
     pause(); //paramos a Y
     exit(0);
    }
    
    pid = fork();
    
    if(pid ==0){
     printf("Soy el proceso Z: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n",getpid(), getppid(),pidAglobal,pidEjec);
     pidZglobal = getpid();
    }else{
      pause();
      //paramos a B y lo preparamos para morir cuando reciba la señal
      exit(0);
    }

    //Z se pausa durante n segundos y despues avisa a A
    signal(SIGALRM,Zfunction);
    alarm(seconds);
    
    pause(); //primer bloqueo hasta que se recibe la alarma
    pause(); //segundo bloqueo hasta que se destruya de manera secuencial el arbol
   
  
    exit(0);


}





