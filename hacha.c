#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>





int getSize(char *file_name){

int df = open(file_name,O_RDONLY);
if(df<0){
    printf("ERROR ABRIENDO ARCHIVO\n");
    exit(0);
}
int size = lseek(df,0,SEEK_END);

if(close(df) == -1){
    printf("ERROR CERRANDO ARCHIVO\n");
    exit(0);
}

return size;
}


void getNumberOfSons(int size,int *sons,int *remainder, int parts){

*sons = size/parts;   //hijos con los bytes determinados por argumento
*remainder = size%parts;  //sobrante(si es mayor que 0 habrá un hijo con menos bytes que el resto)

}

int main(int argc, char *argv[]){
if(argc !=3){
    printf("ERROR: Argumentos inválidos\n");
    exit(0);
}



pid_t father = getpid();
int parts = atoi(argv[2]);
char *file_name = argv[1];

// conseguimos el número total de hijos necesarios y si alguno de ellos ha de tener menos que los otros
int size= getSize(file_name);
int sons,remainder;
getNumberOfSons(size,&sons, &remainder,parts);


         //printf("number of sons: %d, remainder: %d, total: %d\n",sons,remainder, (sons*parts + remainder));
if(remainder > 0){
    sons++;
}
          //printf("number of sons: %d, remainder: %d\n",sons,remainder);

int fd[2];
if(pipe(fd) == -1){
    exit(0);//error creando la tubería; 
}


int df = open(file_name,O_RDONLY);
for(int i =0; i<sons;i++){
    if(i == sons-1 && remainder >0){
        parts = remainder;
    }//si hay sobrante, las partes leídas y escritas han de ser el sobrante en la última iteración

        if(fork() ==0){

        char buffer[parts];//lee la cantidad correspondiente a cada hijo
        close(fd[1]);
        read(fd[0],buffer,parts);
        char name[16];
        if(i<=9){
        snprintf(name,sizeof(name),"%s.h0%d",file_name,i);

        }else{

        snprintf(name,sizeof(name),"%s.h%d",file_name,i);  

        }

        int df0 =  open(name,O_CREAT|O_WRONLY|O_TRUNC,0644);
        write(df0,buffer,parts);
        close(df0);   
        exit(0);
        
        }else{
        
        char buffer[parts];//lee la cantidad correspondiente a cada hijo
        read(df,buffer,parts);  
        write(fd[1],buffer,parts);//envia la parte al hijo a través de la tubería
        wait(NULL);

        }


    }
close(df);
close(fd[0]);
close(fd[1]);


sleep(20);


    exit(0);
}