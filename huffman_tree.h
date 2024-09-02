#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <wchar.h>
#include "minheap.h"
#include "node.h"

Node* create_huffman_tree(MinHeap* heap);
void destroy_huffman_tree(Node* root);
void print_huffman_tree(Node* root);
char* get_huffman_code(Node* root, wchar_t c);

#endif