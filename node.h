#ifndef NODE_H
#define NODE_H

#include <glib.h>
#include <wchar.h>

typedef struct Node {
    wchar_t c;
    unsigned int frequency;
    struct Node* left;
    struct Node* right;
} Node;

Node* create_node(wchar_t c, unsigned int frequency);
void destroy_node(Node* node);
int is_leaf(Node* node);

#endif