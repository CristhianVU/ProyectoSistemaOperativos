// main.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
    printf("%s%s%s%s%s%s\n","   ", argv[0], "    ",argv[1],"   ", argv[2]);
     if (argc > 3 || argc < 2 ) {
       printf("Uso: %s <cpu/memoria/disco> <PID>\n", argv[0]);
      return EXIT_FAILURE;
   }

    char* tipo_estadisticas = argv[1];

    //char* pid_str = argv[2];
    //int pid = atoi(pid_str);

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

    if (child_pid == 0) { // Proceso hijo
        close(pipefd[0]); // Cerramos la parte de lectura del pipe en el proceso hijo

        dup2(pipefd[1], STDOUT_FILENO); // Redirigir la salida estándar al pipe

        if (strcmp(tipo_estadisticas, "cpu") == 0) {
            execlp("./rendimientoCPU", "./rendimientoCPU", "cpu", pid_str, NULL);
        } else if (strcmp(tipo_estadisticas, "memoria") == 0) {
            execlp("./obtener_porcentajes", "./obtener_porcentajes", "memoria", NULL);
        } else if (strcmp(tipo_estadisticas, "disco") == 0) {
            execlp("./rendimientoDisco", "./rendimientoDisco", "disco", argv[2], NULL);
        } else {
            printf("Tipo de estadísticas no válido: %s\n", tipo_estadisticas);
            exit(EXIT_FAILURE);
        }
    } else { // Proceso padre
        close(pipefd[1]); // Cerramos la parte de escritura del pipe en el proceso padre

        printf("Estadísticas para %s con PID %d:\n", tipo_estadisticas, pid);
        char buffer[1024];
        ssize_t bytes_read;
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, bytes_read);
        }

        close(pipefd[0]); // Cerramos la parte de lectura del pipe en el proceso padre
    }

    return EXIT_SUCCESS;
}