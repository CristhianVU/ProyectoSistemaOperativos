#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    FILE *diskstats;
    char line[100];

    diskstats = popen("cat /proc/diskstats", "r");
    if (diskstats == NULL) {
        perror("Error abriendo el archivo");
        return 1;
    }

    while (fgets(line, sizeof(line), diskstats)) {
        printf("%s", line);
    }

    pclose(diskstats);
    return 0;
}