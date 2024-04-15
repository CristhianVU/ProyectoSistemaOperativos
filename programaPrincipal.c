#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
    if (argc > 4 || argc < 2 ) {
       printf("Uso: %s <cpu/memoria/disco> <PID>\n", argv[0]);
      return EXIT_FAILURE;
   }

    char* tipo_estadisticas = argv[1];

    char* pid_str ;
    int pid ;

    if(argv[2]==NULL){

     pid_str = argv[1];
    pid = atoi(pid_str);

    }else{

    pid_str = argv[2];
    pid = atoi(pid_str);

    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("Error al crear el pipe");
        return EXIT_FAILURE;
    }

    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("Error en fork");
        return EXIT_FAILURE;
    }

    if (child_pid == 0) { 
        close(pipefd[0]); 

        dup2(pipefd[1], STDOUT_FILENO); 
        if (strcmp(tipo_estadisticas, "cpu") == 0) {
            execlp("./rendimientoCPU", "./rendimientoCPU", "cpu", pid_str, NULL);
        } else if (strcmp(tipo_estadisticas, "memoria") == 0) {
           if(argv[2]==NULL){
            execlp("./rendimientoMemoria", "./rendimientoMemoria", "memoria",pid_str, NULL);
           }else{
            execlp("./rendimientoMemoria", "./rendimientoMemoria", "memoria",pid_str, argv[3], NULL);
           }
        } else if (strcmp(tipo_estadisticas, "disco") == 0) {
            execlp("./rendimientoDisco", "./rendimientoDisco", "disco", argv[2], NULL);
        } else {
            printf("Tipo de estadísticas no válido: %s\n", tipo_estadisticas);
            exit(EXIT_FAILURE);
        }
    } else { 
        close(pipefd[1]); 

        printf("Estadísticas para %s con PID %d:\n", tipo_estadisticas, pid);
        char buffer[1024];
        ssize_t bytes_read;
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, bytes_read);
        }

        close(pipefd[0]); 
    }

    return EXIT_SUCCESS;
}