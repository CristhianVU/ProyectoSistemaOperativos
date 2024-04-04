#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    FILE *meminfo;
    char line[100];

    meminfo = popen("cat /proc/meminfo | grep '^MemTotal\\|^MemFree'", "r");
    if (meminfo == NULL) {
        perror("Error abriendo el archivo");
        return 1;
    }

    while (fgets(line, sizeof(line), meminfo)) {
        printf("%s", line);
    }

    pclose(meminfo);
    return 0;
}