#include <wchar.h>
#include <stdlib.h>
#include "dictionary.h"
#include "node.h"
#include "code_list.h"
#include "huffman_tree.h"

/**
 * Crea una lista de nodos con los codigos de huffman para cada caracter en un diccionario
 *
 * @param dict El diccionario de donde se obtienen los caracteres
 * @param huffman_root La raiz del arbol de huffman construido con el diccionario
 * @param capacity La cantidad de nodos maxima en la lista
 * @return Un puntero a la lista de nodos
 * @note El cliente de la funcion es propietario de la memoria asignada a la lista de nodos
 */
CodeList* create_code_list(Dictionary* dict, Node* huffman_root, int capacity) {

    CodeList* list = (CodeList*)malloc(sizeof(CodeList));
    list -> nodes = (CodeNode**)malloc(sizeof(CodeNode*) * capacity);
    list -> capacity = capacity;
    list -> size = 0;

    for (int i = 0; i < dict -> size; i++) {
        wchar_t c = dict -> keys[i];
        char* code = get_huffman_code(huffman_root, c);
        insert_code_node(list, c, code);
    }

    return list;
}

/**
 * Inserta un nodo en la lista de codigos
 *
 * @param list La lista donde se inserta el nodo
 * @param c El caracter del nodo
 * @param code El codigo del nodo
 * @note Si la lista esta llena, no se agrega el nodo a la lista
 */
void insert_code_node(CodeList* list, wchar_t c, char* code) {

    if (list -> size == list -> capacity) {
        return;
    }

    CodeNode* node = (CodeNode*)malloc(sizeof(CodeNode));
    node -> c = c;
    node -> code = code;

    list -> nodes[list -> size] = node;
    list -> size++;
}

/**
 * Libera la memoria de un nodo
 *
 * @param node Un puntero al nodo a destruir
 */
void destroy_code_node(CodeNode* node) {
    free(node -> code);
    free(node);
}

/**
 * Libera la memoria de una lista de codigos. Destruye
 * tambien todos los nodos de la lista
 *
 * @param list Un puntero a la lista a destruir
 */
void destroy_code_list(CodeList* list) {

    for (int i = 0; i < list -> size; i++) {
        CodeNode* node = list -> nodes[i];
        destroy_code_node(node);
    }

    free(list -> nodes);
    free(list);
}

/**
 * Obtiene el codigo huffman de un caracter en la lista
 *
 * @param list La lista de codigos
 * @param c El caracter a buscar en la lista
 * @return Un puntero al codigo correspondiente para el caracter
 * @note Si el caracter no esta en la lista se retorna NULL
 */
char* get_char_code(CodeList* list, wchar_t c) {

    for (int i = 0; i < list -> size; i++) {

        CodeNode* node = list -> nodes[i];

        if (c == node -> c) {
            return node -> code;
        }
    }

    return NULL;
}