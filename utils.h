#ifndef UTILS_H
#define UTILS_H

#include "minheap.h"
#include "dictionary.h"

Dictionary* create_word_freq_dict(char* file_path);
MinHeap* get_min_heap_from_dict(Dictionary* dict);
void get_parent_directory(char *dir_path, char *parent_dir, int size);
void save_dict_in_file(char* file_path, Dictionary* dict);
Dictionary* get_dict_from_file(char* file_path);

#endif