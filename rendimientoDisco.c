#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 1024

double obtener_porcentaje_total() {
    FILE* pipe = popen("df --output=used,size / | tail -n 1", "r");
    if (!pipe) {
        perror("Error al ejecutar df");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFF_SIZE];
    fgets(buffer, BUFF_SIZE, pipe);
    pclose(pipe);

    double espacio_utilizado, espacio_total;
    sscanf(buffer, "%lf %lf", &espacio_utilizado, &espacio_total);
    double espacio_en_mib = espacio_utilizado / (1024 * 1024);
    return espacio_en_mib;
}

double obtener_porcentaje_libre() {
    FILE* pipe = popen("df --output=avail / | tail -n 1", "r");
    if (!pipe) {
        perror("Error al ejecutar df");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFF_SIZE];
    fgets(buffer, BUFF_SIZE, pipe);
    pclose(pipe);

    double espacio_libre;
    sscanf(buffer, "%lf", &espacio_libre);
    return espacio_libre;
}

int main(int argc, char *argv[]) {
    if (argc != 3 || strcmp(argv[1], "disco") != 0 || (strcmp(argv[2], "-tu") != 0 && strcmp(argv[2], "-tl") != 0)) {
        printf("Uso: %s disco <-tu|-tl>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "-tu") == 0) {
    double espacio_utilizado = obtener_porcentaje_total();
    printf("Espacio total utilizado en el disco: %.2f MiB\n", espacio_utilizado);
    } else {
    double espacio_libre = obtener_porcentaje_libre();
    printf("Total de espacio libre en el disco: %.2f MiB\n", espacio_libre);
    }

    return EXIT_SUCCESS;
}
