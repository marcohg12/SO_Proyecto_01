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

#define FILE_PATH_SIZE 512
#define COMP_DIR_SIZE 512
#define PARENT_DIR_SIZE 512
#define COMMAND_SIZE 512
#define FILE_NAME_SIZE 128

// Estructura para pasar los argumentos a los hilos
typedef struct {
    char file_path[FILE_PATH_SIZE];
    char file_name[FILE_PATH_SIZE];
    char comp_dir_path[COMP_DIR_SIZE];
} thread_args_t;

// Función que comprime un archivo
void* compress_file_thread(void* args) {
    thread_args_t* file_args = (thread_args_t*)args;
    
    compress_file(file_args->file_path, file_args->file_name, file_args->comp_dir_path);
    
    free(file_args); 
    return NULL;
}

void thread_comp(char* dir_path){
    struct dirent *entry;
    char file_path[FILE_PATH_SIZE];
    char comp_dir_path[COMP_DIR_SIZE];
    char parent_dir[PARENT_DIR_SIZE];
    
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    // Se obtiene el directorio padre
    get_parent_directory(dir_path, parent_dir, PARENT_DIR_SIZE);

    // Creamos el directorio donde se guardan los archivos comprimidos
    snprintf(comp_dir_path, sizeof(comp_dir_path), "%s/%s", parent_dir, "parallel_comp_temp");
    mkdir(comp_dir_path, 0755);

    // Arreglo para almacenar los identificadores de los hilos
    pthread_t threads[98];  
    int thread_count = 0;

    // Se itera por los archivos del directorio
    while ((entry = readdir(dir)) != NULL) {

        if (entry->d_name[0] == '.') {
            continue; 
        }

        // Se obtiene la ruta completa de cada archivo
        snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, entry->d_name);

        // Se crea una estructura de argumentos para el hilo
        thread_args_t* args = malloc(sizeof(thread_args_t));
        snprintf(args->file_path, sizeof(args->file_path), "%s", file_path);
        snprintf(args->file_name, sizeof(args->file_name), "%s", entry->d_name);
        snprintf(args->comp_dir_path, sizeof(args->comp_dir_path), "%s", comp_dir_path);

        // Se crea el hilo para comprimir el archivo
        if (pthread_create(&threads[thread_count], NULL, compress_file_thread, args) != 0) {
            perror("pthread_create");
            free(args);
            exit(EXIT_FAILURE);
        }

        thread_count++;
    }

    // Espera a que todos los hilos terminen
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    char dir_name[FILE_NAME_SIZE];
    get_last_name(dir_path, dir_name, sizeof(dir_name));
    char command[COMMAND_SIZE];

    snprintf(command, sizeof(command), "cd %s && tar -cf %s.tar -C %s .", parent_dir, dir_name, "comp_temp");
    system(command);
    // Elimina la carpeta temporal con los archivos comprimidos
    delete_directory(comp_dir_path);

    closedir(dir);
}

void thread_decomp(char* dir_path){}

void thread_descomp(char* dir_path){}


