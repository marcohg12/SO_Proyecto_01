// -----------------------------------------------------------------------------
// Este archivo contiene la version del algoritmo de compresion
// utilizando una modalidad serial
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <wchar.h>
#include <sys/stat.h>
#include <string.h>
#include "serial_comp.h"
#include "utils.h"
#include "dictionary.h"
#include "minheap.h"
#include "huffman_tree.h"
#include "code_list.h"
#define COMMAND_SIZE 512
#define PATH_SIZE 512
#define FILE_NAME_SIZE 128

/**
 * Toma un directorio y comprime todos los archivos del mismo en un nuevo directorio
 *
 * @param dir_path La ruta del directorio a comprimir
 */
void serial_comp(char* dir_path) {

    struct dirent* entry;
    char file_path[PATH_SIZE];
    char comp_dir_path[PATH_SIZE];
    char parent_dir[PATH_SIZE];

    DIR* dir = opendir(dir_path);
    if (dir == NULL) {
        printf("Error: Debe indicar la ruta a un directorio existente\n");
        exit(-1);
    }

    // Obtenemos el directorio padre
    get_parent_directory(dir_path, parent_dir, sizeof(parent_dir));

    // Creamos el directorio donde se guardan los archivos comprimidos
    // El directorio se crea en el directorio padre
    snprintf(comp_dir_path, sizeof(comp_dir_path), "%s/%s", parent_dir, "comp_temp");
    mkdir(comp_dir_path, 0755);

    // Iteramos por los archivos del directorio
    while ((entry = readdir(dir)) != NULL) {

        if (entry -> d_name[0] == '.') {
            continue;
        }

        // Obtenemos la ruta completa de cada archivo
        snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, entry -> d_name);

        // Enviamos el archivo a comprimir
        compress_file(file_path, entry -> d_name, comp_dir_path);
    }

    // Crea un archivo para la carpeta con los archivos comprimidos
    char dir_name[FILE_NAME_SIZE];
    get_last_name(dir_path, dir_name, sizeof(dir_name));

    char command[COMMAND_SIZE];
    snprintf(command, sizeof(command), "cd %s && tar -cf %s.tar -C %s .", parent_dir, dir_name, "comp_temp");
    system(command);

    // Elimina la carpeta temporal con los archivos comprimidos
    delete_directory(comp_dir_path);

    closedir(dir);
}

/**
 * Toma un archivo, lo comprime y guarda el resultado en un directorio indicado
 *
 * @param file_path La ruta completa del archivo a comprimir
 * @param filename EL nombre del archivo a comprimir
 * @param comp_dir_path La ruta del directorio donde se guarda el archivo comprimido
 */
void compress_file(char* file_path, char* filename, char* comp_dir_path) {

    char comp_file_path[PATH_SIZE];
    snprintf(comp_file_path, sizeof(comp_file_path), "%s/%s", comp_dir_path, filename);

    // Obtenemos el diccionario de caracteres para el archivo
    Dictionary* dict = create_word_freq_dict(file_path);

    // Creamos el arbol de huffman para el archivo
    MinHeap* heap = get_min_heap_from_dict(dict);
    Node* root = create_huffman_tree(heap);

    // Creamos una lista con los codigos de huffman para cada caracter
    CodeList* codes = create_code_list(dict, root, dict -> capacity);

    // Guardamos el diccionario en el archivo
    save_dict_in_file(comp_file_path, dict);

    // Codificamos el contenido del archivo
    FILE* input = fopen(file_path, "r");
    FILE* temp_file = tmpfile();
    unsigned char byte = 0;
    int bits = 0;
    unsigned int total_bits = 0;

    // Recorremos el archivo y por cada caracter
    wchar_t c;
    while ((c = fgetwc(input)) != WEOF) {

        // Obtenemos el codigo para el caracter
        char* code = get_char_code(codes, c);
        int code_len = strlen(code);

        // Escribimos el codigo en binario en el archivo temporal
        for (int i = 0; i < code_len; i++) {

            byte = (byte << 1) | (code[i] - '0');

            bits++;
            total_bits++;

            if (bits == 8) {
                fputc(byte, temp_file);
                bits = 0;
                byte = 0;
            }
        }
    }

    // Escribimos los bits restantes en el archivo temporal
    if (bits > 0) {
        byte <<= (8 - bits);
        fputc(byte, temp_file);
    }

    fclose(input);
    rewind(temp_file);

    // Escribimos la cantidad de bits que se encuentran en el archivo comprimido
    // Agregamos un delimitador para marcar donde se termina la cantidad de bits
    FILE* output = fopen(comp_file_path, "a");
    fprintf(output, "%d\n---\n", total_bits);
    fclose(output);

    // Escribimos la codificacion contenida en el archivo temporal en el archivo comprimido
    output = fopen(comp_file_path, "ab");

    unsigned long file_size = get_file_size(temp_file);

    char* temp_data = (char*)malloc(file_size);

    int bytes_read = fread(temp_data, 1, file_size, temp_file);
    fwrite(temp_data, 1, bytes_read, output);

    free(temp_data);

    fclose(output);
    fclose(temp_file);

    destroy_code_list(codes);
    destroy_dictionary(dict);
    destroy_min_heap(heap);
    destroy_huffman_tree(root);
}

/**
 * Toma un directorio y descomprime todos los archivos que contiene en un nuevo directorio
 *
 * @param dir_path La ruta del directorio a descomprimir
 */
void serial_decomp(char* dir_path) {

    struct dirent* entry;
    char file_path[PATH_SIZE];
    char decomp_dir_path[PATH_SIZE];
    char parent_dir[PATH_SIZE];
    char temp_dir_path[PATH_SIZE];
    char comp_filename[FILE_NAME_SIZE];
    char decomp_dir_name[FILE_NAME_SIZE];
    char command[COMMAND_SIZE];

    // Obtenemos el directorio padre
    get_parent_directory(dir_path, parent_dir, sizeof(parent_dir));

    // Volvemos a crear la carpeta temporal con los archivos comprimidos
    snprintf(temp_dir_path, sizeof(temp_dir_path), "%s/%s", parent_dir, "comp_temp");
    mkdir(temp_dir_path, 0755);

    snprintf(command, sizeof(command), "cd %s && tar -xf %s", temp_dir_path, dir_path);
    if (system(command) != 0) {
        delete_directory(temp_dir_path);
        printf("Error: Debe indicar la ruta a un archivo .tar existente\n");
        exit(-1);
    }

    // Creamos el directorio donde se guardan los archivos descomprimidos
    // El directorio se crea en el directorio padre
    get_filename_without_extension(dir_path, comp_filename);
    get_unique_dir_name(comp_filename, decomp_dir_name, sizeof(decomp_dir_name), parent_dir);

    snprintf(decomp_dir_path, sizeof(decomp_dir_path), "%s/%s", parent_dir, decomp_dir_name);
    mkdir(decomp_dir_path, 0755);

    DIR* dir = opendir(temp_dir_path);

    // Iteramos por los archivos del directorio
    while ((entry = readdir(dir)) != NULL) {

        if (entry -> d_name[0] == '.') {
            continue;
        }

        // Obtenemos la ruta completa de cada archivo
        snprintf(file_path, sizeof(file_path), "%s/%s", temp_dir_path, entry -> d_name);

        // Enviamos el archivo a descomprimir
        decompress_file(file_path, entry -> d_name, decomp_dir_path);
    }

    closedir(dir);

    // Eliminamos el directorio temporal
    delete_directory(temp_dir_path);
}

/**
 * Descomprime un archivo y guarda el resultado en un directorio indicado
 *
 * @param file_path La ruta completa del archivo a descomprimir
 * @param filename El nombre del archivo a descomprimir
 * @param decomp_dir_path La ruta del directorio donde se crea el archivo descomprimido
 */
void decompress_file(char* file_path, char* filename, char* decomp_dir_path) {

    char decomp_file_path[FILE_NAME_SIZE];
    snprintf(decomp_file_path, sizeof(decomp_file_path), "%s/%s", decomp_dir_path, filename);

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
    unsigned int bits_to_read = 0;

    // Obtenemos la cantidad de bits del archivo
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

    // Decodificamos los bits del archivo
    while (bits_to_read > 0) {

        if (bits == 0) {
            int c = fgetc(input);
            if (c == EOF) {
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
            }
            else {
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