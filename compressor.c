#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include "serial_comp.h"
#include "fork_comp.h"
#include "thread_comp.h"

int main(int argc, char* argv[]) {

    setlocale(LC_ALL, "");

    int serial_comp_f = 1;
    int fork_comp_f = 0;
    int thread_comp_f = 0;
    struct timespec start, end;
    void (*comp_func)(char*);

    if (argc <= 1) {
        printf("Error: debe indicar la ruta del directorio a comprimir\n");
        printf("Para utilizar el compresor debe ingresar primero la ruta del directorio a comprimir.\n");
        printf("Luego puede indicar el modo de compresión con:\n");
        printf("    1: modo serial\n");
        printf("    2: con fork\n");
        printf("    3: con hilos\n");
        printf("Si no ingresa el tipo el compresor utiliza el modo serial por defecto\n");
        return -1;
    }

    char* dir_path = argv[1];

    if (argc == 3) {

        int comp_type = atoi(argv[2]);

        if (comp_type == 2) {
            fork_comp_f = 1;
            serial_comp_f = 0;
        }
        else if (comp_type == 3) {
            thread_comp_f = 1;
            serial_comp_f = 0;
        }
    }

    if (serial_comp_f) {
        printf("Comprimiendo en modo serial...\n");
        comp_func = serial_comp;
    }
    else if (fork_comp_f) {
        printf("Comprimiendo con fork...\n");
        comp_func = parallel_comp;
    }
    else if (thread_comp_f) {
        printf("Comprimiendo con hilos...\n");
        comp_func = thread_comp;

    }

    clock_gettime(CLOCK_MONOTONIC, &start);

    comp_func(dir_path);

    clock_gettime(CLOCK_MONOTONIC, &end);

    long seconds = end.tv_sec - start.tv_sec;
    long nanoseconds = end.tv_nsec - start.tv_nsec;
    long total_nanoseconds = seconds * 1000000000L + nanoseconds;

    printf("Tiempo de compresión: %ld nanosegundos, o %ld segundos\n", total_nanoseconds, seconds);

    return 0;
}