// -----------------------------------------------------------------------------
// Este archivo contiene funciones auxiliares y de uso general
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <wchar.h>
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
    while((c = fgetwc(file)) != WEOF){
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
MinHeap* get_min_heap_from_dict(Dictionary* dict){

    MinHeap* heap = create_min_heap(1024);
    
    for (int i = 0; i < dict -> size; i++) {

        Node* node = create_node(dict -> keys[i], dict -> values[i]);
        insert_node(heap, node);
    }

    return heap;
}

void get_parent_directory(char *dir_path, char *parent_dir, int size) {

    strncpy(parent_dir, dir_path, size);

    char *last_slash = strrchr(parent_dir, '/');

    if (last_slash != NULL) {

        if (last_slash == parent_dir) {
            parent_dir[1] = '\0';
        } else {
            *last_slash = '\0';
        }
    }
}

void save_dict_in_file(char* file_path, Dictionary* dict){

    FILE *file = fopen(file_path, "w");

    for (int i = 0; i < dict -> size; i++){
        fprintf(file, "%u %d\n", (wint_t)dict -> keys[i], dict -> values[i]);
    }

    fprintf(file, "---\n");

    fclose(file);
}

Dictionary* get_dict_from_file(char* file_path){

    Dictionary *dict = create_dictionary(1024);

    FILE *file = fopen(file_path, "r");

    wint_t c;
    unsigned int value;
    int i;

    while (i = fscanf(file, "%u %d\n", &c, &value) == 2){
        insert_in_dict(dict, (wchar_t)c, value);
    }

    fclose(file);

    return dict;
}