#ifndef DICT_H
#define DICT_H

#include <wchar.h>

typedef struct Dictionary {
    wchar_t* keys;
    unsigned int* values;
    int capacity;
    int size;
} Dictionary;

Dictionary* create_dictionary(int capacity);
void destroy_dictionary(Dictionary* dict);
void insert_or_update(Dictionary* dict, wchar_t c, unsigned int value);
unsigned int get_value(Dictionary* dict, wchar_t c);
void insert_in_dict(Dictionary* dict, wchar_t c, unsigned int value);
void print_dict(Dictionary* dict);

#endif