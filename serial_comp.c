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
    FILE *temp_file = tmpfile();
    unsigned char byte = 0;
    int bits = 0;
    int total_bits = 0;

    // Recorremos el archivo y por cada caracter
    wchar_t c;
    while ((c = fgetwc(input)) != WEOF){
        
        // Obtenemos el codigo para el caracter
        char* code = get_huffman_code(root, c);
        int code_len = strlen(code);
        
        // Escribimos el codigo en binario en el archivo temporal
        for (int i = 0; i < code_len; i++){

            if (code[i] == '0'){
                byte = (byte << 1);
            } 
            else {
                byte = (byte << 1) | 1;
            }

            bits++;
            total_bits++;

            if (bits == 8) {
                fputc(byte, temp_file);
                bits = 0;
                byte = 0;
            }
        }

        free(code);
    }

    // Escribimos los bits restantes en el archivo temporal
    if (bits > 0){
        byte <<= (8 - bits);
        fputc(byte, temp_file);
    }

    fclose(input);
    rewind(temp_file);

    // Escribimos la cantidad de bits que se encuentran en el archivo comprimido
    FILE *output = fopen(comp_file_path, "a");
    fprintf(output, "%d\n---\n", total_bits);
    fclose(output);

    // Escribimos la codificacion en el archivo comprimido
    output = fopen(comp_file_path, "ab");

    char temp_data[256];
    int bytes_read;
    while ((bytes_read = fread(temp_data, 1, sizeof(temp_data), temp_file)) > 0) {
        fwrite(temp_data, 1, bytes_read, output);
    }

    fclose(output);
    fclose(temp_file);

    destroy_dictionary(dict);
    destroy_min_heap(heap);
    destroy_huffman_tree(root);
}

void serial_decomp(char* dir_path){

    struct dirent *entry;
    char file_path[FILE_PATH_SIZE];
    char decomp_dir_path[COMP_DIR_SIZE];
    char parent_dir[PARENT_DIR_SIZE];


    DIR *dir = opendir(dir_path);

    // Obtenemos el directorio padre
    get_parent_directory(dir_path, parent_dir, PARENT_DIR_SIZE);

    // Creamos el directorio donde se guardan los archivos descomprimidos
    // El directorio se crea en el directorio padre
    snprintf(decomp_dir_path, sizeof(decomp_dir_path), "%s/%s", parent_dir, "decomp_temp");
    mkdir(decomp_dir_path, 0755);

    // Iteramos por los archivos del directorio
    while ((entry = readdir(dir)) != NULL) {

        if (entry -> d_name[0] == '.'){
            continue;
        }
        
        // Obtenemos la ruta completa de cada archivo
        snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, entry -> d_name);

        // Enviamos el archivo a descomprimir
        decompress_file(file_path, entry -> d_name, decomp_dir_path);
    }

    closedir(dir);
}

void decompress_file(char* file_path, char* filename, char* decomp_dir_path){

    char decomp_file_path[FILE_PATH_SIZE];
    snprintf(decomp_file_path, sizeof(decomp_file_path), "%s/%s", decomp_dir_path, filename);

    wprintf(L"Descomprimiendo: %s y guardando en %s\n", file_path, decomp_file_path);

    // Obtenemos el diccionario de frecuencias del archivo
    Dictionary* dict = get_dict_from_file(file_path);

    // Construimos el arbol de huffman para el archivo
    MinHeap* heap = get_min_heap_from_dict(dict);
    Node* root = create_huffman_tree(heap);

    // Decodificamos el archivo
    FILE* input = fopen(file_path, "rb");
    FILE* output = fopen(decomp_file_path, "w");

    char line[256];
    char* mark = "---\n";
    int mark_len = strlen(mark);
    int bits_to_read = 0;

    // Vamos al archivo donde empieza la codificacion binaria
    while (fgets(line, sizeof(line), input) != NULL) {
        if (strstr(line, mark) != NULL) {
            fscanf(input, "%d\n---\n", &bits_to_read);
            break;
        }
    }
    
    int bits = 0;
    unsigned char byte = 0;
    int output_count = 0;
    Node* temp_root = root;

    while (bits_to_read > 0) {
    
        if (bits == 0) {
            int c = fgetc(input);
            if (c == EOF){
                break;
            }
            byte = (unsigned char)c;
            bits = 8;
        }

        if (is_leaf(temp_root)) {
            fputwc(temp_root -> c, output);
            temp_root = root;
        }
        else {

          bits--;
          bits_to_read--;

          if (byte & (1 << (bits))) {
            temp_root = temp_root -> right;
          } else {
            temp_root = temp_root -> left;
          }
        }
    }

    fclose(input);
    fclose(output);

    destroy_dictionary(dict);
    destroy_min_heap(heap);
    destroy_huffman_tree(root);
}