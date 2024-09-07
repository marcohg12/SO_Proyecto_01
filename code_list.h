#ifndef CODE_LIST_H
#define CODE_LIST_H

#include <wchar.h>
#include "dictionary.h"
#include "node.h"

typedef struct CodeNode {
    wchar_t c;
    char* code;
} CodeNode;

typedef struct CodeList {
    CodeNode** nodes;
    int capacity;
    int size;
} CodeList;

CodeList* create_code_list(Dictionary* dict, Node* huffman_root, int capacity);
void destroy_code_list(CodeList* list);
void destroy_code_node(CodeNode* node);
char* get_char_code(CodeList* list, wchar_t c);
void insert_code_node(CodeList* list, wchar_t c, char* code);

#endif