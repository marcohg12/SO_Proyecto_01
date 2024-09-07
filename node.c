// -----------------------------------------------------------------------------
// Este archivo contiene funciones para el manejo de nodos
// -----------------------------------------------------------------------------

#include <stdlib.h>
#include <wchar.h>
#include "node.h"

/**
 * Crea un nodo
 *
 * @param c El caracter para el nodo
 * @param frequency La frecuencia del caracter
 * @return Un puntero al nodo
 * @note El cliente es dueño del puntero al nodo, y es el encargado de administrar la memoria del nodo
 */
Node* create_node(wchar_t c, unsigned int frequency) {
    Node* node = (Node*)malloc(sizeof(Node));
    node -> c = c;
    node -> frequency = frequency;
    node -> left = NULL;
    node -> right = NULL;
    return node;
}

/**
 * Destruye un nodo, libera la memoria asignada al nodo
 *
 * @param node El nodo a destruir
 */
void destroy_node(Node* node) {
    free(node);
}

/**
 * @return Retorna 1 si el nodo es hoja (los punteros a los hijos son NULL) o 0 en el caso contrario
 */
int is_leaf(Node* node) {
    return (node -> left == NULL && node -> right == NULL);
}