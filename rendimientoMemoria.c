#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h> 

#define PROC_STAT "/proc/stat"
#define PROC_STAT_PID "/proc/%d/stat"
#define PROC_STATM_PID "/proc/%d/statm"
#define BUFF_SIZE 1024


double obtener_memoria_total();

void obtener_porcentaje_memoria_virtual() {
DIR* proc_dir = opendir("/proc");
if (!proc_dir) {
perror("Error al abrir el directorio /proc");
exit(EXIT_FAILURE);
}

struct dirent* entry;
printf("PID\tNombre\tPorcentaje de Memoria Virtual\n");

while ((entry = readdir(proc_dir)) != NULL) {
int pid = atoi(entry->d_name);
if (pid > 0) {
char proc_statm_path[BUFF_SIZE];
snprintf(proc_statm_path, BUFF_SIZE, PROC_STATM_PID, pid);

FILE* file = fopen(proc_statm_path, "r");
if (file) {
unsigned long size;
fscanf(file, "%*s %lu", &size); 

double memoria_kb = size * sysconf(_SC_PAGESIZE) / 1024.0;

char proc_stat_path[BUFF_SIZE];
snprintf(proc_stat_path, BUFF_SIZE, PROC_STAT_PID, pid);
FILE* stat_file = fopen(proc_stat_path, "r");
if (stat_file) {
char comm[BUFF_SIZE];
fscanf(stat_file, "%*d %s", comm); 

double memoria_total_kb = obtener_memoria_total();
double porcentaje_memoria = (memoria_kb / memoria_total_kb) * 100.0;
printf("%d\t%s\t%.2f%%\n", pid, comm, porcentaje_memoria);

fclose(stat_file);
}
fclose(file);
}
}
}

closedir(proc_dir);
}


void obtener_porcentaje_memoria_real() {
DIR* proc_dir = opendir("/proc");
if (!proc_dir) {
perror("Error al abrir el directorio /proc");
exit(EXIT_FAILURE);
}

struct dirent* entry;
printf("PID\tNombre\tPorcentaje de Memoria Real\n");

while ((entry = readdir(proc_dir)) != NULL) {
int pid = atoi(entry->d_name);
if (pid > 0) {
char proc_statm_path[BUFF_SIZE];
snprintf(proc_statm_path, BUFF_SIZE, PROC_STATM_PID, pid);

FILE* file = fopen(proc_statm_path, "r");
if (file) {
unsigned long resident_pages;
fscanf(file, "%lu", &resident_pages); 

double memoria_kb = resident_pages * sysconf(_SC_PAGESIZE) / 1024.0;


char proc_stat_path[BUFF_SIZE];
snprintf(proc_stat_path, BUFF_SIZE, PROC_STAT_PID, pid);
FILE* stat_file = fopen(proc_stat_path, "r");
if (stat_file) {
char comm[BUFF_SIZE];
fscanf(stat_file, "%*d %s", comm); 


double memoria_total_kb = obtener_memoria_total();
double porcentaje_memoria = (memoria_kb / memoria_total_kb) * 100.0;
printf("%d\t%s\t%.2f%%\n", pid, comm, porcentaje_memoria);

fclose(stat_file);
}
fclose(file);
}
}
}

closedir(proc_dir);
}


double obtener_memoria_total() {
FILE* meminfo = fopen("/proc/meminfo", "r");
if (!meminfo) {
perror("Error al abrir /proc/meminfo");
exit(EXIT_FAILURE);
}

char buffer[BUFF_SIZE];
double memoria_total_kb = 0.0;

while (fgets(buffer, BUFF_SIZE, meminfo)) {
if (strncmp(buffer, "MemTotal:", 9) == 0) {
unsigned long memoria_total;
sscanf(buffer + 9, "%lu", &memoria_total);
memoria_total_kb = memoria_total / 1024.0; 
break;
}
}

fclose(meminfo);
return memoria_total_kb;
}


void obtener_porcentaje_memoria_virtual_pid(int pid) {
    char proc_statm_path[BUFF_SIZE];
    snprintf(proc_statm_path, BUFF_SIZE, PROC_STATM_PID, pid);

    FILE* file = fopen(proc_statm_path, "r");
    if (!file) {
        printf("No se pudo abrir el archivo /proc/%d/statm\n", pid);
        return;
    }

    unsigned long size;
    if (fscanf(file, "%*s %lu", &size) != 1) {
        printf("Error al leer el archivo /proc/%d/statm\n", pid);
        fclose(file);
        return;
    }


    double memoria_kb = size * sysconf(_SC_PAGESIZE) / 1024.0;

 
    char proc_stat_path[BUFF_SIZE];
    snprintf(proc_stat_path, BUFF_SIZE, PROC_STAT_PID, pid);
    FILE* stat_file = fopen(proc_stat_path, "r");
    if (!stat_file) {
        printf("No se pudo abrir el archivo /proc/%d/stat\n", pid);
        fclose(file);
        return;
    }

    char comm[BUFF_SIZE];
    if (fscanf(stat_file, "%*d %s", comm) != 1) {
        printf("Error al leer el archivo /proc/%d/stat\n", pid);
        fclose(stat_file);
        fclose(file);
        return;
    }

  
    double memoria_total_kb = obtener_memoria_total();
    double porcentaje_memoria = (memoria_kb / memoria_total_kb) * 100.0;
    printf("%d\t%s\t%.2f%%\n", pid, comm, porcentaje_memoria);

    fclose(stat_file);
    fclose(file);
}

int main(int argc, char *argv[]) {


    if (argc < 2 || argc > 4 || (strcmp(argv[1], "memoria") != 0)) {
        printf("Uso: %s memoria [-r] [-v PID]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argc == 2 || (argc == 3 && strcmp(argv[2], "-r") == 0)) {

        obtener_porcentaje_memoria_real();
    } else if (argc == 4 && strcmp(argv[2], "-v") == 0 ) {
  
        int pid = atoi(argv[3]);
        if (pid <= 0) {
            printf("El PID debe ser un número entero positivo.\n");
            return EXIT_FAILURE;
        }
      
        printf("Porcentaje de Memoria Virtual para el PID %d:\n", pid);
        if(strcmp(argv[2], "-v") == 0){
            obtener_porcentaje_memoria_virtual_pid(pid);
        }
    } else if (strcmp(argv[2], "memoria") == 0){
            obtener_porcentaje_memoria_virtual();
        }
        else {
        printf("Uso: %s memoria [-r] [-v PID]\n", argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


