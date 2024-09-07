// -----------------------------------------------------------------------------
// Este archivo contiene funciones para el uso de diccionarios
// -----------------------------------------------------------------------------

#include <wchar.h>
#include <stdlib.h>
#include "dictionary.h"

/**
 * Crea un diccionario con la capacidad indicada
 *
 * @param capacity Un entero que indica la capacidad de pares llave-valor que puede guardar el diccionario
 * @return Retorna un puntero al diccionario
 * @note El cliente es propietario del puntero, y debe encargarse de administrar la memoria del mismo
 */
Dictionary* create_dictionary(int capacity) {
    Dictionary* dict = (Dictionary*)malloc(sizeof(Dictionary));
    dict -> keys = (wchar_t*)malloc(sizeof(wchar_t) * capacity);
    dict -> values = (unsigned int*)malloc(sizeof(unsigned int) * capacity);
    dict -> capacity = capacity;
    dict -> size = 0;
}

/**
 * Libera la memoria de un diccionario
 *
 * @param dict Un puntero al diccionario
 */
void destroy_dictionary(Dictionary* dict) {
    free(dict -> keys);
    free(dict -> values);
    free(dict);
}

/**
 * Recibe una llave y un valor. Si la llave ya existe en el diccionario, aumenta en
 * uno su valor correspondiente, si no, inserta la llave y el valor dado en el diccionario
 *
 * @param dict El diccionario donde se inserta o actualiza la llave y su valor
 * @param c La llave a buscar o insertar en el diccionario
 * @param value El valor para colocar en el diccionario si la llave se inserta
 * @note Si la llave va a insertarse y no hay espacio en el diccionario, entonces no se inserta
 */
void insert_or_update(Dictionary* dict, wchar_t c, unsigned int value) {

    for (int i = 0; i < dict -> size; i++) {
        if (c == dict -> keys[i]) {
            dict -> values[i]++;
            return;
        }
    }

    if (dict -> size == dict -> capacity) {
        return;
    }

    dict -> keys[dict -> size] = c;
    dict -> values[dict -> size] = value;
    dict -> size++;
}

/**
 * Obtiene el valor para una llave en el diccionario
 *
 * @param dict El diccionario donde se hace la busqueda de la llave
 * @param c La llave por la cual se busca el valor
 * @return Retorna el valor de la llave
 * @note Se asume que la llave existe en el diccionario
 */
unsigned int get_value(Dictionary* dict, wchar_t c) {

    for (int i = 0; i < dict -> size; i++) {
        if (c == dict -> keys[i]) {
            return dict -> values[i];
        }
    }
}

/**
 * Inserta un par llave-valor en el diccionario
 *
 * @param dict El diccionario donde se inserta la llave y el valor
 * @param c La llave del par
 * @param value El valor del par
 * @note Se asume que el par no existe en el diccionario. La funcion no verifica si la llave ya existe en el diccionario
 */
void insert_in_dict(Dictionary* dict, wchar_t c, unsigned int value) {

    if (dict -> size == dict -> capacity) {
        return;
    }

    dict -> keys[dict -> size] = c;
    dict -> values[dict -> size] = value;
    dict -> size++;
}

/**
 * Imprime el contenido de un diccionario (llaves y sus valores)
 *
 * @param dict Un puntero al diccionario a imprimir
 */
void print_dict(Dictionary* dict) {

    for (int i = 0; i < dict -> size; i++) {
        wprintf(L"%lc %d\n", dict -> keys[i], dict -> values[i]);
    }
}