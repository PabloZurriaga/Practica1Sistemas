#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

pid_t pidAglobal;
pid_t pidZglobal; //para la destruccion secuencial del arbol;
pid_t pidYglobal;
pid_t pidXglobal;
pid_t pidEjec;

void Bfunction(){
}

void Zfunction(){
  //envia señal  a A para que ejecute pstree
   kill(pidAglobal,SIGUSR1);
}


void Afunction(){ //A recibe la señal, crea un hijo que hace el pstree, espera a que este muera y luego hara un wait(NULL) en el main
   pid_t p =fork();
   if(p == 0){
    char pid_str[16];
    snprintf(pid_str, sizeof(pid_str), "%d", pidEjec); //pasamos pidEjec a string para que solo se imprima el arbol de procesos del programa

    execlp("pstree", "pstree", "-c", pid_str, NULL);
    exit(0); 
   }
  wait(NULL);
}





int main(int argc, char *argv[]){
    
    if(argc!= 2){
      exit(0);
    }
    
    int seconds=atoi(argv[1]);

    signal(SIGUSR2,Bfunction);
    
    printf("Soy el proceso ejec: mi pid es %d\n", getpid());//esto solo lo imprime ejec
   
    pidEjec = getpid();

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
        kill(pid,SIGUSR2);
        wait(NULL);
        printf("Soy A (%d) y muero\n", getpid());
        exit(0);
      }
    }else{ 
      wait(NULL);//hacemos que ejec espere a que su hijo directo finalice
      printf("Soy ejec (%d) y muero\n", getpid());
      exit(0);
    }
 
    //esto lo hace solo B
    pid =fork(); //crea a X

    if(pid ==0){
     printf("Soy el proceso X: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n",getpid(), getppid(),pidAglobal,pidEjec);
     pause(); //paramos a X
     printf("Soy X (%d) y muero\n", getpid());
     exit(0);
    }
    pidXglobal = pid;
    pid = fork();

    if(pid ==0){
     printf("Soy el proceso Y: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n",getpid(), getppid(),pidAglobal,pidEjec);
     pause(); //paramos a Y
     printf("Soy Y (%d) y muero\n", getpid());
     exit(0);
    }
    pidYglobal = pid;
    
    pid = fork();
    
    if(pid ==0){
     printf("Soy el proceso Z: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n",getpid(), getppid(),pidAglobal,pidEjec);
     pidZglobal = getpid();
    }else{
      pidZglobal = pid;
      pause();
      //paramos a B y lo preparamos para morir cuando reciba la señal
      kill(pidZglobal,SIGUSR2);
      wait(NULL);
      kill(pidYglobal,SIGUSR2);
      wait(NULL);
      kill(pidXglobal,SIGUSR2);
      wait(NULL);
      printf("Soy B (%d) y muero\n", getpid());
      exit(0);
    }

    //Z se pausa durante n segundos y despues avisa a A
    signal(SIGALRM,Zfunction);
    alarm(seconds);
    
    pause(); //primer bloqueo hasta que se recibe la alarma
    pause(); //segundo bloqueo hasta que se destruya de manera secuencial el arbol
   
    printf("Soy Z (%d) y muero\n", getpid());
    exit(0);


}