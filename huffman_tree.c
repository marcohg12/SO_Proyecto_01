// -----------------------------------------------------------------------------
// Este archivo contiene funciones para el manejo de arboles de huffman
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "huffman_tree.h"
#include "minheap.h"
#include "node.h"

/**
 * Crea un arbol de huffman a partir de los nodos de un min heap
 *
 * @param heap Un min heap con los nodos hoja del arbol
 * @return Un puntero al nodo raiz del arbol de huffman
 * @note Esta operacion modifica el contenido del min heap. Despues de la operacion el min heap queda vacio
 */
Node* create_huffman_tree(MinHeap* heap) {

    while (heap -> size > 1) {

        Node* node1 = extract_min(heap);
        Node* node2 = extract_min(heap);

        Node* new_node = create_node((wchar_t)'^', node1 -> frequency + node2 -> frequency);
        new_node -> left = node1;
        new_node -> right = node2;

        insert_node(heap, new_node);
    }

    return extract_min(heap);
}

/**
 * Destruye y libera la memoria de un arbol de huffman
 *
 * @param root El nodo raiz del arbol de huffman
 */
void destroy_huffman_tree(Node* root) {

    if (root == NULL) {
        return;
    }

    if (is_leaf(root)) {
        destroy_node(root);
        return;
    }

    destroy_huffman_tree(root -> left);
    destroy_huffman_tree(root -> right);
    destroy_node(root);
}

void print_huffman_tree(Node* root) {

    if (root == NULL) {
        return;
    }

    if (is_leaf(root)) {
        wprintf(L"%lc: %d\n", root -> c, root -> frequency);
        return;
    }
    else {
        wprintf(L"%lc: %d\n", root -> c, root -> frequency);
    }

    print_huffman_tree(root -> left);
    print_huffman_tree(root -> right);
}

/**
 * Construye recursivamente el codigo huffman para un caracter dado. Esta es una funcion auxiliar
 *
 * @param root El nodo actual en el arbol de huffman
 * @param c El caracter para el que se esta armando el codigo
 * @param code Un puntero a un arreglo donde se va almacenando el codigo para el caracter
 * @param index El indice en el arreglo donde se va a insertar el siguiente valor para el codigo
 * @return Un entero para indicar cuando se encuentra el nodo hoja y detener la recursion
 * @note La funcion asume que el caracter se encuentra en un nodo hoja del arbol
 */
int get_huffman_code_aux(Node* root, wchar_t c, char* code, int index) {

    if (root == NULL) {
        return 0;
    }

    if (is_leaf(root)) {

        if (c == root -> c) {
            code[index] = '\0';
            return 1;
        }
        return 0;
    }

    code[index] = '0';
    if (get_huffman_code_aux(root -> left, c, code, index + 1)) {
        return 1;
    }

    code[index] = '1';
    get_huffman_code_aux(root -> right, c, code, index + 1);
}

/**
 * Contruye el codigo huffman para un caracter con el arbol dado
 *
 * @param root El nodo raiz del arbol de huffman
 * @param c El caracter para el que se construye el codigo
 * @return Un puntero a un arreglo de caracteres con el codigo huffman para el caracter
 * @note Se asume que el caracter existe en las hojas del arbol
 * @note El cliente es dueño del puntero al arreglo, es el encargado de liberar la memoria cuando termine de usarlo
 */
char* get_huffman_code(Node* root, wchar_t c) {
    char* code = (char*)malloc(sizeof(char) * 64);
    get_huffman_code_aux(root, c, code, 0);
    return code;
}