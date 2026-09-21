#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc,char *argv[]){


    if(argc != 3){
        printf("ERROR: número de argumentos erróneo\n"); //comprobamos que hay dos argumentos(x e y) además de ./malla
    }else{
    
        int x = atoi(argv[1]);
        int y = atoi(argv[2]);


        pid_t pid = fork(); //creamos hijo del proceso padre, que será asignado 0 para el hijo y el numero de PID al padre


        //el hijo ignora la creación de nuevos hijos para las columnas(todas han de provenir del proceso padre)

        for(int i =0; i < x -1; i++){ //restamos uno a x e y porque ya hemos creado un hijo y sino el resultado de filas y columnas sera x+1,y+1
            if(pid != 0  && pid != -1){
                pid = fork();  //los procesos hijos a de las columnas a excepcion del primero nacen aqui, y no se duplicaran con el bucle, solo lo hara el padre
            }

        }
        
        for(int i =0; i < y -1; i++){
            if(pid ==0){
                pid = fork();  //los procesos hijos a de las filas a excepcion del primero nacen aqui se duplicaran con el bucle, y el padre no
            }

        }

    }


    sleep(20);
    exit(0);
}