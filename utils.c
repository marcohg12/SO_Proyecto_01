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







