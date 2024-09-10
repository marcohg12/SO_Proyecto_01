#ifndef THREAD_COMP_H
#define THREAD_COMP_H

void thread_comp(char* dir_path);
void* compress_file_thread(void* args);
void thread_decomp(char* dir_path);
void decompress_file_thread(void* args);

#endif