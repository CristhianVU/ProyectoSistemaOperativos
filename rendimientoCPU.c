#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PROC_STAT "/proc/stat"
#define PROC_STAT_PID "/proc/%d/stat"
#define BUFF_SIZE 1024

// Función para obtener el porcentaje de utilización total del CPU
double obtener_porcentaje_total() {
    FILE* file = fopen(PROC_STAT, "r");
    if (!file) {
        perror("Error al abrir /proc/stat");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFF_SIZE];
    fgets(buffer, BUFF_SIZE, file);
    fclose(file);

    char* token = strtok(buffer, " ");
    token = strtok(NULL, " "); // Ignorar "cpu"
    double user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;

    sscanf(token, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice);

    double total_idle = idle + iowait;
    double total_non_idle = user + nice + system + irq + softirq + steal;
    double total = total_idle + total_non_idle;
    return (total_non_idle / total) * 100.0;
}

// Función para obtener el porcentaje de utilización para un proceso específico
double obtener_porcentaje_proceso(int pid) {
    char proc_stat_path[BUFF_SIZE];
    snprintf(proc_stat_path, BUFF_SIZE, PROC_STAT_PID, pid);

    FILE* file = fopen(proc_stat_path, "r");
    if (!file) {
        perror("Error al abrir /proc/<PID>/stat");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFF_SIZE];
    fgets(buffer, BUFF_SIZE, file);
    fclose(file);

    char* token = strtok(buffer, " ");
    int i;
    for (i = 0; i < 13; ++i) {
        token = strtok(NULL, " ");
    }

    int utime, stime;
    sscanf(token, "%d %d", &utime, &stime);

    double total_time = utime + stime;
    double seconds = sysconf(_SC_CLK_TCK); // Get clock ticks per second
    double time_in_seconds = seconds / 100.0;

    return (total_time / time_in_seconds) * 100.0;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <cpu|proceso> <PID>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "cpu") == 0) {
        double porcentaje_total = obtener_porcentaje_total();
        printf("Porcentaje de utilización total del CPU: %.2f%%\n", porcentaje_total);
    } else if (strcmp(argv[1], "proceso") == 0) {
        int pid = atoi(argv[2]);
        if (pid <= 0) {
            
            printf("El PID debe ser un entero positivo.\n");
            return EXIT_FAILURE;
        }

        double porcentaje_proceso = obtener_porcentaje_proceso(pid);
        printf("Porcentaje de utilización para el proceso con PID %d: %.2f%%\n", pid, porcentaje_proceso);
    } else {
        printf("Argumento no válido.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/*int main(int argc, char *argv[]) {
    if (argc != 3 || (strcmp(argv[1], "cpu") != 0 && strcmp(argv[1], "proceso") != 0)) {
        printf("Uso: %s <cpu|proceso> <PID>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "cpu") == 0) {
        double porcentaje_total = obtener_porcentaje_total();
        printf("Porcentaje de utilización total del CPU: %.2f%%\n", porcentaje_total);
    } else if (strcmp(argv[1], "proceso") == 0) {
        int pid = atoi(argv[2]);
        if (pid <= 0) {
            printf("El PID debe ser un entero positivo.\n");
            return EXIT_FAILURE;
        }

        double porcentaje_proceso = obtener_porcentaje_proceso(pid);
        printf("Porcentaje de utilización para el proceso con PID %d: %.2f%%\n", pid, porcentaje_proceso);
    }

    return EXIT_SUCCESS;
}*/