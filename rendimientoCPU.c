#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    FILE *cpuinfo;
    char line[100];

    cpuinfo = popen("cat /proc/stat | grep '^cpu '", "r");
    if (cpuinfo == NULL) {
        perror("Error abriendo el archivo");
        return 1;
    }

    while (fgets(line, sizeof(line), cpuinfo)) {
        printf("%s", line);
    }

    pclose(cpuinfo);
    return 0;
}