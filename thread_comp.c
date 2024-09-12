// -----------------------------------------------------------------------------
// Este archivo contiene la version del algoritmo de compresion
// utilizando hilos de la biblioteca pthread
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "thread_comp.h"
#include "serial_comp.h"
#include "utils.h"
#include "dictionary.h"
#include "minheap.h"
#include "huffman_tree.h"

#define PATH_SIZE 512
#define COMMAND_SIZE 512
#define FILE_NAME_SIZE 128
#define MAX_THREADS 100

// Estructura para pasar los argumentos a los hilos para comprimir
typedef struct {
    char file_path[PATH_SIZE];
    char file_name[PATH_SIZE];
    char comp_dir_path[PATH_SIZE];
} thread_args_comp;

// Estructura para pasar los argumentos a los hilos para descomprimir
typedef struct {
    char file_path[PATH_SIZE];
    char file_name[PATH_SIZE];
    char decomp_dir_path[PATH_SIZE];
} thread_args_decomp;


/**
 * Funcion auxiliar que comprime el archivo con un hilo
 *
 * @param aargs Un puntero a un struct con los parametros que necesita la funcion compress_file
 * @note El struct se debe castear a un puntero a void al pasarlo por parametro
 */
void* compress_file_thread(void* args) {

    thread_args_comp* file_args = (thread_args_comp*)args;

    compress_file(file_args -> file_path, file_args -> file_name, file_args -> comp_dir_path);

    free(file_args);

    return NULL;
}

/**
 * Funcion auxiliar para descomprimir un archivo con un hilo
 *
 * @param aargs Un puntero a un struct con los parametros que necesita la funcion decompress_file
 * @note El struct se debe castear a un puntero a void al pasarlo por parametro
 */
void* decompress_file_thread(void* args) {

    thread_args_comp* file_args = (thread_args_comp*)args;

    decompress_file(file_args -> file_path, file_args -> file_name, file_args -> comp_dir_path);

    free(file_args);

    return NULL;
}

/**
 * Toma un directorio y comprime todos los archivos del mismo en un nuevo directorio
 *
 * @param dir_path La ruta del directorio a comprimir
 */
void thread_comp(char* dir_path) {

    struct dirent* entry;
    char file_path[PATH_SIZE];
    char comp_dir_path[PATH_SIZE];
    char parent_dir[PATH_SIZE];

    DIR* dir = opendir(dir_path);
    if (dir == NULL) {
        printf("Error: Debe indicar la ruta a un directorio existente\n");
        exit(-1);
    }

    // Obtenemos el directorio padre
    get_parent_directory(dir_path, parent_dir, sizeof(parent_dir));

    // Creamos el directorio temporal donde se guardan los archivos comprimidos
    // El directorio se crea en el directorio padre
    snprintf(comp_dir_path, sizeof(comp_dir_path), "%s/%s", parent_dir, "comp_temp");
    mkdir(comp_dir_path, 0755);

    // Se crea un arreglo para almacenar los identificadores de los hilos
    pthread_t threads[MAX_THREADS];
    int thread_count = 0;

    // Se itera por los archivos del directorio
    while ((entry = readdir(dir)) != NULL) {

        if (entry->d_name[0] == '.') {
            continue;
        }

        // Se obtiene la ruta completa de cada archivo
        snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, entry->d_name);

        // Se crea una estructura de argumentos para el hilo
        thread_args_comp* args = malloc(sizeof(thread_args_comp));
        snprintf(args -> file_path, sizeof(args -> file_path), "%s", file_path);
        snprintf(args -> file_name, sizeof(args -> file_name), "%s", entry -> d_name);
        snprintf(args -> comp_dir_path, sizeof(args -> comp_dir_path), "%s", comp_dir_path);

        // Se crea el hilo para comprimir el archivo
        pthread_create(&threads[thread_count], NULL, compress_file_thread, args);

        thread_count++;
    }

    // Se espera a que todos los hilos terminen
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    // Crea un archivo para la carpeta con los archivos comprimidos
    char dir_name[FILE_NAME_SIZE];
    get_last_name(dir_path, dir_name, sizeof(dir_name));

    char command[COMMAND_SIZE];
    snprintf(command, sizeof(command), "cd %s && tar -cf %s.tar -C %s .", parent_dir, dir_name, "comp_temp");
    system(command);

    // Elimina la carpeta temporal con los archivos comprimidos
    delete_directory(comp_dir_path);

    closedir(dir);
}

/**
 * Toma un directorio y descomprime todos los archivos del mismo en un nuevo directorio
 *
 * @param dir_path La ruta del directorio a descomprimir
 */
void thread_decomp(char* dir_path) {

    struct dirent* entry;
    char file_path[PATH_SIZE];
    char decomp_dir_path[PATH_SIZE];
    char parent_dir[PATH_SIZE];
    char temp_dir_path[PATH_SIZE];
    char comp_filename[FILE_NAME_SIZE];
    char decomp_dir_name[FILE_NAME_SIZE];
    char command[COMMAND_SIZE];

    // Se obtiene el directorio padre
    get_parent_directory(dir_path, parent_dir, sizeof(parent_dir));

    // Se crea la carpeta temporal con los archivos comprimidos
    snprintf(temp_dir_path, sizeof(temp_dir_path), "%s/%s", parent_dir, "comp_temp");
    mkdir(temp_dir_path, 0755);

    snprintf(command, sizeof(command), "cd %s && tar -xf %s", temp_dir_path, dir_path);
    if (system(command) != 0) {
        delete_directory(temp_dir_path);
        printf("Error: Debe indicar la ruta a un archivo .tar existente\n");
        exit(-1);
    }

    // Se crea el directorio donde se guardan los archivos descomprimidos
    // El directorio se crea en el directorio padre
    get_filename_without_extension(dir_path, comp_filename);
    get_unique_dir_name(comp_filename, decomp_dir_name, sizeof(decomp_dir_name), parent_dir);

    snprintf(decomp_dir_path, sizeof(decomp_dir_path), "%s/%s", parent_dir, decomp_dir_name);
    mkdir(decomp_dir_path, 0755);

    DIR* dir = opendir(temp_dir_path);

    // Se crea un arreglo para almacenar los identificadores de los hilos
    pthread_t threads[MAX_THREADS];
    int thread_count = 0;

    // Se itera por los archivos del directorio
    while ((entry = readdir(dir)) != NULL) {

        if (entry -> d_name[0] == '.') {
            continue;
        }

        // Se obtiene la ruta completa de cada archivo
        snprintf(file_path, sizeof(file_path), "%s/%s", temp_dir_path, entry -> d_name);

        // Se crea una estructura de argumentos para el hilo
        thread_args_decomp* args = malloc(sizeof(thread_args_decomp));
        snprintf(args -> file_path, sizeof(args -> file_path), "%s", file_path);
        snprintf(args -> file_name, sizeof(args -> file_name), "%s", entry -> d_name);
        snprintf(args -> decomp_dir_path, sizeof(args -> decomp_dir_path), "%s", decomp_dir_path);

        // Se Crea el hilo para descomprimir el archivo
        pthread_create(&threads[thread_count], NULL, decompress_file_thread, args);

        thread_count++;
        if (thread_count >= MAX_THREADS) {
            break;
        }
    }

    // Se espera a que todos los hilos terminen
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    closedir(dir);

    // Se elimina la carpeta temporal con los archivos extraÃ­dos
    delete_directory(temp_dir_path);
}

