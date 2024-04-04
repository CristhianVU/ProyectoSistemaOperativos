#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork(); // Crear un nuevo proceso hijo
    if (pid == -1) {
        perror("Error en fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Proceso hijo
        // Código del proceso hijo
        printf("Proceso auxiliar en ejecución. PID: %d\n", getpid());
        sleep(60); // Esperar 60 segundos
        printf("Proceso auxiliar terminado.\n");
        exit(EXIT_SUCCESS);
    } else { // Proceso padre
        // No hacemos nada en el proceso padre
        printf("Proceso padre\n");
    }

    return EXIT_SUCCESS;
}