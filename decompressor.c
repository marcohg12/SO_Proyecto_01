#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include "serial_comp.h"
#include "thread_comp.h"
#include "fork_comp.h"

int main(int argc, char* argv[]) {

    setlocale(LC_ALL, "");

    int serial_decomp_f = 1;
    int fork_decomp_f = 0;
    int thread_decomp_f = 0;
    struct timespec start, end;
    void (*decomp_func)(char*);

    if (argc <= 1) {
        printf("Error: debe indicar la ruta del directorio a descomprimir\n");
        return 1;
    }

    char* dir_path = argv[1];

    if (argc == 3) {

        int decomp_type = atoi(argv[2]);

        if (decomp_type == 2) {
            fork_decomp_f = 1;
            serial_decomp_f = 0;
        }
        else if (decomp_type == 3) {
            thread_decomp_f = 1;
            serial_decomp_f = 0;
        }
    }

    if (serial_decomp_f) {
        printf("Descomprimiendo en modo serial...\n");
        decomp_func = serial_decomp;
    }
    else if (fork_decomp_f) {
        printf("Descomprimiendo con fork...\n");
        decomp_func = parallel_decomp;
    }
    else if (thread_decomp_f) {
        printf("Descomprimiendo con hilos...\n");
        decomp_func = thread_decomp;

    }

    clock_gettime(CLOCK_MONOTONIC, &start);

    decomp_func(dir_path);

    clock_gettime(CLOCK_MONOTONIC, &end);

    long seconds = end.tv_sec - start.tv_sec;
    long nanoseconds = end.tv_nsec - start.tv_nsec;
    long total_nanoseconds = seconds * 1000000000L + nanoseconds;

    printf("Tiempo de descompresión: %ld nanosegundos, o %ld segundos\n", total_nanoseconds, seconds);

    return 0;
}