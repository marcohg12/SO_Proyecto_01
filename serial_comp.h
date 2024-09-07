#ifndef SERIAL_COMP_H
#define SERIAL_COMP_H

void serial_comp(char* dir_path);
void compress_file(char* file_path, char* filename, char* comp_dir_path);
void serial_decomp(char* dir_path);
void decompress_file(char* file_path, char* filename, char* decomp_dir_path);

#endif