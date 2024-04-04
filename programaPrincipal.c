#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s [cpu/memoria/disco]\n", argv[0]);
        return 1;
    }

    int pipefd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    if (pipe(pipefd) == -1) {
        perror("Error creando el pipe");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        perror("Error al crear el proceso hijo");
        return 1;
    } else if (pid == 0) {  // Proceso hijo
        close(pipefd[0]);  // Cerrar el extremo de lectura del pipe

        // Redirigir la salida estándar al extremo de escritura del pipe
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("Error en dup2");
            return 1;
        }

        close(pipefd[1]);  // Cerrar el extremo de escritura del pipe

        // Ejecutar el programa correspondiente al argumento pasado
        if (strcmp(argv[1], "cpu") == 0) {
            execlp("./rendimientoCPU", "./rendimientoCPU", NULL);
        } else if (strcmp(argv[1], "memoria") == 0) {
            execlp("./rendimientoMemoria", "./rendimientoMemoria", NULL);
        } else if (strcmp(argv[1], "disco") == 0) {
            execlp("./rendimientoDisco", "./rendmientoDisco", NULL);
        } else {
            printf("Recurso no válido. Use 'cpu', 'memoria' o 'disco'.\n");
            return 1;
        }
    } else {  // Proceso padre
        close(pipefd[1]);  // Cerrar el extremo de escritura del pipe

        // Leer la salida del hijo del extremo de lectura del pipe
        while ((bytes_read = read(pipefd[0], buffer, BUFFER_SIZE - 1)) > 0) {
            buffer[bytes_read] = '\0';  // Añadir el terminador nulo
            printf("%s", buffer);
        }

        if (bytes_read == -1) {
            perror("Error leyendo desde el pipe");
            return 1;
        }

        close(pipefd[0]);  // Cerrar el extremo de lectura del pipe
        wait(NULL);  // Esperar a que el hijo termine
    }

    return 0;
}