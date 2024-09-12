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
#define COMMAND_SIZE 512
#define FILE_NAME_SIZE 128
#define PATH_SIZE 512

/**
 * Toma un directorio y comprime todos los archivos del mismo en un nuevo directorio
 * utilizando procesos
 *
 * @param dir_path La ruta del directorio a comprimir
 */
void parallel_comp(char* dir_path) {

    struct dirent* entry;
    char file_path[FILE_PATH_SIZE];
    char comp_dir_path[COMP_DIR_SIZE];
    char parent_dir[PARENT_DIR_SIZE];
    pid_t pid;

    DIR* dir = opendir(dir_path);
    if (dir == NULL) {
        printf("Error: Debe indicar la ruta a un directorio existente\n");
        exit(-1);
    }

    // Obtenemos el directorio padre
    get_parent_directory(dir_path, parent_dir, sizeof(parent_dir));

    // Creamos el directorio donde se guardan los archivos comprimidos
    // El directorio se crea en el directorio padre
    snprintf(comp_dir_path, sizeof(comp_dir_path), "%s/%s", parent_dir, "comp_temp");
    mkdir(comp_dir_path, 0755);

    // Iteramos por los archivos del directorio
    while ((entry = readdir(dir)) != NULL) {

        if (entry -> d_name[0] == '.') {
            continue;
        }

        // Obtenemos la ruta completa de cada archivo
        snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, entry -> d_name);

        // Creamos un nuevo proceso para comprimir el archivo
        pid = fork();

        if (pid == 0) {
            // Proceso hijo
            compress_file(file_path, entry -> d_name, comp_dir_path);
            exit(0);
        }
        else if (pid < 0) {
            printf("Error: Ha ocurrido un problema al usar fork\n");
            exit(-1);
        }
    }

    // Esperamos a que todos los procesos hijos terminen
    while (wait(NULL) > 0);

    // Crea un archivo para la carpeta con los archivos comprimidos
    char dir_name[FILE_NAME_SIZE];
    get_last_name(dir_path, dir_name, sizeof(dir_name));

    char command[COMMAND_SIZE];
    snprintf(command, sizeof(command), "cd %s && tar -cf %s.tar -C %s .", parent_dir, dir_name, "comp_temp");
    system(command);

    // Se elimina el directorio temporal
    delete_directory(comp_dir_path);

    closedir(dir);
}

/**
 * Descomprime los archivos de un directorio utilizando procesos
 *
 * @param dir_path La ruta del directorio a comprimir
 */
void parallel_decomp(char* dir_path) {

    struct dirent* entry;
    char file_path[PATH_SIZE];
    char decomp_dir_path[PATH_SIZE];
    char parent_dir[PATH_SIZE];
    char temp_dir_path[PATH_SIZE];
    char comp_filename[FILE_NAME_SIZE];
    char decomp_dir_name[FILE_NAME_SIZE];
    char command[COMMAND_SIZE];
    pid_t pid;

    // Obtenemos el directorio padre
    get_parent_directory(dir_path, parent_dir, sizeof(parent_dir));

    // Volvemos a crear la carpeta temporal con los archivos comprimidos
    snprintf(temp_dir_path, sizeof(temp_dir_path), "%s/%s", parent_dir, "comp_temp");
    mkdir(temp_dir_path, 0755);

    snprintf(command, sizeof(command), "cd %s && tar -xf %s", temp_dir_path, dir_path);
    if (system(command) != 0) {
        delete_directory(temp_dir_path);
        printf("Error: Debe indicar la ruta a un archivo .tar existente\n");
        exit(-1);
    }

    // Creamos el directorio donde se guardan los archivos descomprimidos
    get_filename_without_extension(dir_path, comp_filename);
    get_unique_dir_name(comp_filename, decomp_dir_name, sizeof(decomp_dir_name), parent_dir);

    snprintf(decomp_dir_path, sizeof(decomp_dir_path), "%s/%s", parent_dir, decomp_dir_name);
    mkdir(decomp_dir_path, 0755);

    DIR* dir = opendir(temp_dir_path);

    // Iteramos por los archivos del directorio
    while ((entry = readdir(dir)) != NULL) {

        if (entry->d_name[0] == '.') {
            continue;
        }

        // Obtenemos la ruta completa de cada archivo
        snprintf(file_path, sizeof(file_path), "%s/%s", temp_dir_path, entry->d_name);

        // Creamos un proceso hijo para descomprimir el archivo
        pid = fork();

        if (pid == 0) {
            // Este es el proceso hijo
            decompress_file(file_path, entry -> d_name, decomp_dir_path);
            exit(0);
        }
        else if (pid < 0) {
            printf("Error: Ha ocurrido un problema al usar fork\n");
            exit(-1);
        }
    }

    // Esperamos a que todos los procesos hijos terminen
    while (wait(NULL) > 0);

    closedir(dir);

    // Eliminamos el directorio temporal
    delete_directory(temp_dir_path);
}
