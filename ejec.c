#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

pid_t pidAglobal;

int main(int argc, char argv[]){
    char letters[3]= "XYZ";
    printf("Soy el proceso ejec: mi pid es %d\n", getpid());//esto solo lo imprime ejec
   
    pid_t pidEjec = getpid();

    pid_t pid = fork(); //el proceso A comienza su ejecución aqí
   
    if(pid == 0){
      pidAglobal = getpid();

      printf("Soy el proceso A: mi pid es %d. Mi padre es %d\n",getpid(), getppid());

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
    pid =fork();

    if(pid==0){

       printf("Soy el proceso X: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n",getpid(), getppid(),pidAglobal,pidEjec);
       
       pause(); //paramos a X
    }else{

      pid==fork();

      if(pid ==0){
      printf("Soy el proceso Y: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n",getpid(), getppid(),pidAglobal,pidEjec);
      pause();
    }else{
      pid = fork();
      pause(); //paramos a B
    }
    }

   if(pid ==0){  //solo lo debe de ejecutar Z
  
   }
   

   sleep(20);
    exit(0);


}
       //  printf("Soy el proceso %c: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n",letters[i],getpid(), getppid(),pidAglobal,pidEjec);





