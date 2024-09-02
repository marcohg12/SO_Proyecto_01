// -----------------------------------------------------------------------------
// Este archivo contiene funciones para el manejo de min heaps
// -----------------------------------------------------------------------------

#include "minheap.h"
#include "node.h"

/**
 * Crea un min heap con la capacidad indicada, inicializado con tamaño 0 y el array de nodos vacio
 * 
 * @param capacity La cantidad de nodos que puede guardar el min heap
 * @return Un puntero al min heap creado
 * @note El cliente es dueño del puntero al min heap, debe liberar el min heap cuando lo termine de usar
 * @note Se asume que la capacidad es positiva
 */
MinHeap* create_min_heap(int capacity){

    MinHeap *heap = (MinHeap*)malloc(sizeof(MinHeap));

    heap -> nodes = (Node**)malloc(capacity * sizeof(Node*));
    heap -> size = 0;
    heap -> capacity = capacity;

    return heap;
}

/**
 * Destruye y libera la memoria de un min heap. Los nodos en el min heap tambien se destruyen
 * 
 * @param heap El min heap a liberar
 * @note Se asume que el puntero no es NULL
 */
void destroy_min_heap(MinHeap* heap){
    
    for (int i = 0; i < heap -> size; i++) {
        destroy_node(heap -> nodes[i]);
    }

    free(heap -> nodes);
    free(heap);
}

/**
 * Inserta un nodo en el min heap, manteniendo la condicion de min heap despues de la insercion
 * 
 * @param heap El heap donde se inserta el nodo
 * @param node El nodo a insertar en el min heap
 * @note Se asume que los punteros al heap y al nodo no son NULL
 * @note Si no hay mas espacio en el heap, no se inserta el nodo
 */
void insert_node(MinHeap *heap, Node *node) {

    if (heap -> size == heap -> capacity) {
        return;
    }

    int index = heap -> size;

    heap -> nodes[index] = node;

    while (index != 0) {

        int parent = (index - 1) / 2;

        if (heap -> nodes[index] -> frequency >= heap -> nodes[parent] -> frequency){
            break;
        }

        Node *temp = heap -> nodes[index];

        heap -> nodes[index] = heap -> nodes[parent];
        heap -> nodes[parent] = temp;

        index = parent;
    }

    heap -> size++;
}

/**
 * Organiza el arreglo de nodos para mantener la condicion de min heap a partir de un indice dado
 * 
 * @param heap El heap donde se organiza el arreglo de nodos
 * @param index El indice en la lista de nodos donde se verifica y organiza la condicion
 */
void min_heapify(MinHeap *heap, int index) {

    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap -> size && heap -> nodes[left] -> frequency < heap -> nodes[smallest] -> frequency) {
        smallest = left;
    }

    if (right < heap -> size && heap -> nodes[right] -> frequency < heap -> nodes[smallest] -> frequency) {
        smallest = right;
    }

    if (smallest != index) {

        Node *temp = heap -> nodes[index];
        heap -> nodes[index] = heap -> nodes[smallest];
        heap -> nodes[smallest] = temp;
        min_heapify(heap, smallest);
    }
}

/**
 * Retorna la raiz del min heap, es decir, el nodo con la menor frecuencia.
 * Si no hay nodos en el min heap entonces retorna NULL
 * 
 * @param heap El heap de donde se extrae el nodo
 * @return Un puntero al nodo con menor frecuencia
 * @note El nodo se elimina del min heap con esta operacion
 * @note El cliente es dueño del puntero al nodo, es el encargado de administrar le memoria del nodo
 */
Node* extract_min(MinHeap *heap) {

    if (heap -> size == 0){
        return NULL;
    }

    Node *root = heap -> nodes[0];

    heap -> nodes[0] = heap -> nodes[(heap -> size) - 1];
    heap -> size--;

    min_heapify(heap, 0);

    return root;
}