#ifndef UTILS_H
#define UTILS_H

#include "minheap.h"
#include "dictionary.h"

Dictionary* create_word_freq_dict(char* file_path);
MinHeap* get_min_heap_from_dict(Dictionary* dict);
void get_parent_directory(char* dir_path, char* parent_dir, int parent_dir_size);
void save_dict_in_file(char* file_path, Dictionary* dict);
Dictionary* get_dict_from_file(char* file_path);
void get_last_name(const char* path, char* buffer, int buffer_size);
void delete_directory(char* dir_path);
void get_unique_dir_name(char* base_name, char* dir_name, int dir_name_size, char* parent_dir);
void get_filename_without_extension(char* file_path, char* output);

#endif