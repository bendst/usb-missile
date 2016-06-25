#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>


int main(int argc, char *argv[]) {
    argc--;
    argv++;

    if (argc != 1) {
        printf("keine device angegeben");
        return EXIT_FAILURE;
    }

    int fd = open(argv[0], O_RDWR);
    if (fd < 0 ) {
        perror("open");
        return EXIT_FAILURE;
    }

    int in = 0;
    int r = 0;
    char buf[125] = {};

    while (1) {
        printf("writing to device %s: ", argv[0]);
        r = fscanf(stdin, "%x", &in);
        if (r < 0) {
            break;
        }

        r = write(fd, &in, sizeof(int));
        if (r < 0) {
            perror("write");
            return EXIT_FAILURE;
        }


        printf("reading from device %s: ", argv[0]);
        r = read(fd, buf, sizeof(buf));
        if (r < 0) {
            perror("read");
            return EXIT_SUCCESS;
        }


        for (int i = 0; i < r; ++i) {
            printf("%x", (int)buf[i]);
        }

        printf("\n");
        memset(buf, 0, sizeof(buf));
    }

    close(fd);
    return 0;
}

