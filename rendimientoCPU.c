#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



#define PROC_STAT "/proc/stat"
#define PROC_STAT_PID "/proc/%d/stat"
#define BUFF_SIZE 1024

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
    token = strtok(NULL, " "); 
    double user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;

    sscanf(token, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice);

    double total_idle = idle + iowait;
    double total_non_idle = user + nice + system + irq + softirq + steal;
    double total = total_idle + total_non_idle;
    return (total_non_idle / total) * 100.0;
}

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

    unsigned long utime, stime;
    sscanf(buffer, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %lu %lu", &utime, &stime);

    long ticks_per_second = sysconf(_SC_CLK_TCK);
    if (ticks_per_second <= 0) {
        fprintf(stderr, "Error: sysconf(_SC_CLK_TCK) falló\n");
        exit(EXIT_FAILURE);
    }

    double total_time_seconds = (utime + stime) / (double)ticks_per_second;

    double porcentaje = (total_time_seconds * 100.0); 

    return porcentaje;
}

int main(int argc, char *argv[]) {
   
   
    if (argc != 3 || (strcmp(argv[1], "cpu") != 0 && strcmp(argv[1], "proceso") != 0)) {
        printf("Uso: %s <cpu|proceso> <PID>\n", argv[0]);
        return EXIT_FAILURE;
    }
       
    if (strcmp(argv[2], "cpu") == 0) {
        double porcentaje_total = obtener_porcentaje_total();
        printf("Porcentaje de utilización total del CPU: %.2f%%\n", porcentaje_total);
    } else {
        int pid = atoi(argv[2]);
        if (pid <= 0) {
            printf("El PID debe ser un entero positivo.\n");
            return EXIT_FAILURE;
        }

        double porcentaje_proceso = obtener_porcentaje_proceso(pid);
        printf("Porcentaje de utilización para el proceso con PID %d: %.2f%%\n", pid, porcentaje_proceso);
    }

    return EXIT_SUCCESS;
}