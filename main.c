#include <stdio.h>
#include <locale.h>
#include "utils.h"
#include "minheap.h"
#include "huffman_tree.h"
#include "dictionary.h"
#include "wchar.h"
#include "serial_comp.h"

int main() {

    setlocale(LC_ALL, "");

    //serial_comp("/home/marco/Escritorio/Proyectos/SO_Proyecto_01/tests/books");
    serial_decomp("/home/marco/Escritorio/Proyectos/SO_Proyecto_01/tests/books.tar");

    return 0;
}