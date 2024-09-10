// -----------------------------------------------------------------------------
// Este archivo contiene funciones auxiliares y de uso general
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <wchar.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include "minheap.h"
#include "node.h"
#include "dictionary.h"

/**
 * Retorna un diccionario con las frecuencias de cada caracter en el archivo
 *
 * @param file_path La ruta del archivo donde se leen los caracteres
 * @return El diccionario donde las llaves son los caracteres y los valores son las frecuencias de los caracteres
 * @note La funcion asume que el archivo existe en la ruta dada
 * @note EL cliente es dueño del puntero al diccionario, al terminar de usarlo debe destruilo
 */
Dictionary* create_word_freq_dict(const char* file_path) {

    Dictionary* dict = create_dictionary(1024);

    FILE* file = fopen(file_path, "r");

    wint_t c;
    while ((c = fgetwc(file)) != WEOF) {
        insert_or_update(dict, (wchar_t)c, 1);
    }

    fclose(file);

    return dict;
}

/**
 * Retorna un min heap de nodos construido a partir de un diccionario
 * Cada nodo se forma de un caracter y se frecuencia
 *
 * @param dict El diccionario con el que se construyen los nodos
 * @return Un min heap donde los elementos son nodos ordenados por la frecuencia del caracter
 * @note Los nodos solo se inicializan con el caracter y la frecuencia, los demas atributos se dejan en NULL
 * @note El cliente es dueño del puntero al min heap, al terminar de usarlo debe destruirlo
 */
MinHeap* get_min_heap_from_dict(Dictionary* dict) {

    MinHeap* heap = create_min_heap(1024);

    for (int i = 0; i < dict -> size; i++) {

        Node* node = create_node(dict -> keys[i], dict -> values[i]);
        insert_node(heap, node);
    }

    return heap;
}

/**
 * Obtiene el directorio padre de un directorio
 *
 * @param dir_path La ruta del directorio hijo
 * @param parent_dir Un buffer donde se guarda la ruta del directorio padre
 * @param parent_dir_size El tamaño del buffer para guardar el directorio padre
 */
void get_parent_directory(char* dir_path, char* parent_dir, int parent_dir_size) {

    strncpy(parent_dir, dir_path, parent_dir_size);

    char* last_slash = strrchr(parent_dir, '/');

    if (last_slash != NULL) {

        if (last_slash == parent_dir) {
            parent_dir[1] = '\0';
        }
        else {
            *last_slash = '\0';
        }
    }
}

/**
 * Guarda el contenido de un diccionario en un archivo
 *
 * @param file_path La ruta del archivo donde se guarda el diccionario
 * @param dict El diccionario que se guarda en el archivo
 * @note Al final del archivo se agrega la linea '---\n' para delimitar donde terminan los datos del diccionario
 * @note Esta funcion elimina los contenidos previos del archivo
 */
void save_dict_in_file(char* file_path, Dictionary* dict) {

    FILE* file = fopen(file_path, "w");

    for (int i = 0; i < dict->size; i++) {
        fprintf(file, "%u %d\n", (wint_t)dict->keys[i], dict->values[i]);
    }

    fprintf(file, "---\n");

    fclose(file);
}

/**
 * Construye un diccionario a partir de un archivo
 *
 * @param dir_path La ruta del archivo de donde se obtienen los datos del diccionario
 * @return Un puntero al diccionario creados con los pares almacenados en el archivo
 * @note Se asume que el archivo existe
 */
Dictionary* get_dict_from_file(char* file_path) {

    Dictionary* dict = create_dictionary(1024);

    FILE* file = fopen(file_path, "r");

    wint_t c;
    unsigned int value;
    int i;

    // Se leen los pares hasta que se encuentre el delimitador
    while (i = fscanf(file, "%u %d\n", &c, &value) == 2) {
        insert_in_dict(dict, (wchar_t)c, value);
    }

    fclose(file);

    return dict;
}

/**
 * Obtiene el nombre del ultimo elemento (archivo o directorio) en una ruta
 *
 * @param path La ruta de donde se obtiene el nombre
 * @param buffer Un buffer donde se guarda el nombre del ultimo elemento de la ruta
 * @param buffer_size El tamaño del buffer
 */
void get_last_name(char* path, char* buffer, int buffer_size) {

    char* last_slash = strrchr(path, '/');

    char* last_name_start;

    if (last_slash != NULL) {
        last_name_start = (last_slash + 1);
    }
    else {
        last_name_start = path;
    }

    int last_name_length = strlen(last_name_start);

    strncpy(buffer, last_name_start, buffer_size - 1);
    buffer[buffer_size - 1] = '\0';
}

/**
 * Elimina los contenidos de un directorio y el propio directorio
 *
 * @param dir_path La ruta del directorio a eliminar
 * @note Se asume que el directorio solo contiene archivos y no otros directorios
 */
void delete_directory(char* dir_path) {

    struct dirent* entry;
    DIR* dir = opendir(dir_path);

    // Se recorren los archivos del directorio y se eliminan
    while ((entry = readdir(dir))) {

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);
        remove(full_path);
    }

    closedir(dir);

    // Se elimina el directorio
    rmdir(dir_path);
}

/**
 * Verifica si un directorio existe
 *
 * @param dir_path La ruta del directorio que se quiere verificar
 * @return 1 si el directorio existe, 0 en el caso contrario
 */
int dir_exists(char* dir_path) {
    struct stat st;
    return (stat(dir_path, &st) == 0 && S_ISDIR(st.st_mode));
}

/**
 * Obtiene un nombre de directorio unico para un nombre base
 *
 * @param base_name El nombre base para el directorio
 * @param dir_name Un buffer donde se guarda el nombre unico para el directorio
 * @param dir_name_size El tamaño del buffer donde se guarda el nombre unico para el directorio
 * @param parent_dir El directorio padre donde se verifica el nombre unico del directorio
 */
void get_unique_dir_name(char* base_name, char* dir_name, int dir_name_size, char* parent_dir) {

    char temp_dir[512];
    unsigned int counter = 1;
    int result;

    int temp_dir_size = sizeof(temp_dir);

    snprintf(dir_name, dir_name_size, "%s", base_name);
    snprintf(temp_dir, temp_dir_size, "%s/%s", parent_dir, dir_name);

    // Verificamos si el directorio es unico en el directorio padre
    while (dir_exists(temp_dir)) {

        // Si no es unico, agregamos un contador el nombre
        // El contador se incrementa hasta encontrar un nombre unico
        snprintf(dir_name, dir_name_size, "%s (%d)", base_name, counter);
        snprintf(temp_dir, temp_dir_size, "%s/%s", parent_dir, dir_name);

        counter++;
    }
}

/**
 * Obtiene el nombre de un archivo sin la extension
 *
 * @param file_path La ruta del archivo
 * @param buffer El buffer donde se almacena el nombre del archivo
 */
void get_filename_without_extension(char* file_path, char* buffer) {

    char* filename;
    char* dot;

    filename = strrchr(file_path, '/');

    if (filename == NULL) {
        filename = (char*)file_path;
    }
    else {
        filename++;
    }

    dot = strrchr(filename, '.');


    if (dot != NULL) {
        int len = dot - filename;
        strncpy(buffer, filename, len);
        buffer[len] = '\0';
    }
    else {
        strcpy(buffer, filename);
    }
}

/**
 * Obtiene el tamaño de un archivo
 *
 * @param file Un puntero al archivo
 * @return El tamaño del archivo
 * @note El puntero del archivo apunta al inicio tras usar la funcion
 */
unsigned long get_file_size(FILE* file) {
    fseek(file, 0, SEEK_END);
    unsigned long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}