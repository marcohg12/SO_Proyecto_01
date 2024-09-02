#ifndef MINHEAP_H
#define MINHEAP_H

#include "node.h"

typedef struct MinHeap {
    Node **nodes;     
    int size;      
    int capacity;
} MinHeap;

MinHeap* create_min_heap(int capacity);
void destroy_min_heap(MinHeap* heap);
Node* extract_min(MinHeap *heap);
void min_heapify(MinHeap *heap, int index);
void insert_node(MinHeap *heap, Node *node);

#endif