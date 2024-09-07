// -----------------------------------------------------------------------------
// Este archivo contiene la version del algoritmo de compresion
// utilizando la funcion fork para la clonacion de procesos
// -----------------------------------------------------------------------------


#include <stdio.h>
#include <dirent.h>
#include <wchar.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "serial_comp.h"
#include "utils.h"
#include "dictionary.h"
#include "minheap.h"
#include "huffman_tree.h"
#define FILE_PATH_SIZE 256
#define COMP_DIR_SIZE 256
#define PARENT_DIR_SIZE 256


void parallel_comp(char* dir_path){
    struct dirent *entry;
    char file_path[FILE_PATH_SIZE];
    char comp_dir_path[COMP_DIR_SIZE];
    char parent_dir[PARENT_DIR_SIZE];
    pid_t pid;

    DIR *dir = opendir(dir_path);

    // Obtenemos el directorio padre
    get_parent_directory(dir_path, parent_dir, PARENT_DIR_SIZE);

    // Creamos el directorio donde se guardan los archivos comprimidos
    // El directorio se crea en el directorio padre
    snprintf(comp_dir_path, sizeof(comp_dir_path), "%s/%s", parent_dir, "parallel_comp_temp");
    mkdir(comp_dir_path, 0755);

    // Iteramos por los archivos del directorio
    while ((entry = readdir(dir)) != NULL) {

        if (entry -> d_name[0] == '.'){
            continue;
        }
        
        // Obtenemos la ruta completa de cada archivo
        snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, entry -> d_name);

        // Creamos un nuevo proceso para comprimir el archivo
        pid = fork();

        if (pid == 0) {
            // Proceso hijo
            compress_file(file_path, entry->d_name, comp_dir_path);
            exit(0); 
        } else if (pid < 0) {
            // Error al crear el proceso
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    // Esperamos a que todos los procesos hijos terminen
    while (wait(NULL) > 0);

    closedir(dir);
}