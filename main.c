#include <stdio.h>
#include <locale.h>
#include "utils.h"
#include "minheap.h"
#include "huffman_tree.h"
#include "dictionary.h"
#include "wchar.h"

void serial_comp(char* dir_path);
void serial_decomp(char* dir_path);

int main() {

    setlocale(LC_ALL, "");

    //serial_comp("/home/marco/Escritorio/Proyectos/SO_Proyecto_01/Libros");
    serial_decomp("/home/marco/Escritorio/Proyectos/SO_Proyecto_01/Libros.tar");

    return 0;
}