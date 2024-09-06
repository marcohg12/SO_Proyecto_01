// -----------------------------------------------------------------------------
// Este archivo contiene la version del algoritmo de compresion
// utilizando una modalidad serial
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <dirent.h>
#include <wchar.h>
#include <sys/stat.h>
#include "serial_comp.h"
#include "utils.h"
#include "dictionary.h"
#include "minheap.h"
#include "huffman_tree.h"
#define FILE_PATH_SIZE 256
#define COMP_DIR_SIZE 256
#define PARENT_DIR_SIZE 256

void serial_comp(char* dir_path){
    
    struct dirent *entry;
    char file_path[FILE_PATH_SIZE];
    char comp_dir_path[COMP_DIR_SIZE];
    char parent_dir[PARENT_DIR_SIZE];


    DIR *dir = opendir(dir_path);

    // Obtenemos el directorio padre
    get_parent_directory(dir_path, parent_dir, PARENT_DIR_SIZE);

    // Creamos el directorio donde se guardan los archivos comprimidos
    // El directorio se crea en el directorio padre
    snprintf(comp_dir_path, sizeof(comp_dir_path), "%s/%s", parent_dir, "comp_temp");
    mkdir(comp_dir_path, 0755);

    // Iteramos por los archivos del directorio
    while ((entry = readdir(dir)) != NULL) {

        if (entry -> d_name[0] == '.'){
            continue;
        }
        
        // Obtenemos la ruta completa de cada archivo
        snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, entry -> d_name);

        // Enviamos el archivo a comprimir
        compress_file(file_path, entry -> d_name, comp_dir_path);
    }

    closedir(dir);
}

void compress_file(char* file_path, char* filename, char* comp_dir_path){

    char comp_file_path[FILE_PATH_SIZE];
    snprintf(comp_file_path, sizeof(comp_file_path), "%s/%s", comp_dir_path, filename);

    wprintf(L"Comprimiendo: %s y guardando en %s\n", file_path, comp_file_path);

    // Obtenemos el diccionario de caracteres para el archivo
    Dictionary* dict = create_word_freq_dict(file_path);

    // Creamos el arbol de huffman para el archivo
    MinHeap* heap = get_min_heap_from_dict(dict);
    Node* root = create_huffman_tree(heap);

    // Guardamos el diccionario en el archivo
    save_dict_in_file(comp_file_path, dict);

    // Codificamos el contenido del archivo
    FILE *input = fopen(file_path, "r");
    FILE *output = fopen(comp_file_path, "ab");
    unsigned char byte = 0;
    int bits = 0;

    // Recorremos el archivo y por cada caracter
    wchar_t c;
    while ((c = fgetwc(input)) != WEOF){
        
        // Obtenemos el codigo para el caracter
        char* code = get_huffman_code(root, c);
        int code_len = strlen(code);
        
        // Escribimos el codigo en binario en el archivo comprimido
        for (int i = 0; i < code_len; i++){

            if (code[i] == '0'){
                byte = (byte << 1);
            } 
            else {
                byte = (byte << 1) | 1;
            }

            bits++;

            if (bits == 8) {
                fputc(byte, output);
                bits = 0;
                byte = 0;
            }
        }

        free(code);
    }

    // Escribimos los bits restantes
    if (bits > 0){
        bits <<= (8 - bits);
        fputc(bits, output);
    }

    fclose(input);
    fclose(output);

    destroy_dictionary(dict);
    destroy_min_heap(heap);
    destroy_huffman_tree(root);
}