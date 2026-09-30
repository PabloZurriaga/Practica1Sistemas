#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int getSons(){
    int res=0;
}



int getSize(char *file_name){
int df = open(file_name,O_RDONLY);
if(df<0){
    printf("ERROR ABRIENDO ARCHIVO\n");
}
int size = lseek(df,0,SEEK_END);

if(close(df) == -1){
    printf("ERROR CERRANDO ARCHIVO\n");
}
return size;
}





int main(int argc, char *argv[]){
if(argc !=3){
    printf("ERROR: Argumentos inválidos\n");
    exit(0);
}
int parts = atoi(argv[2]);
char *file_name = argv[1];
printf("%s,%d\n",file_name,parts);

int size= getSize(file_name);
printf("%d\n", size);
int numberOfSons = size/parts+ size%parts;
printf("number of sons: %d\n",numberOfSons);




    exit(0);
}