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



int main(int argc, char *argv[]){

    
    int x,y;
    if(argc != 3){
        printf("ERROR: Argumentos inválidos\n");
        exit(0);
    }
    
    parse_args(argv,&x,&y);





    exit(0);
}