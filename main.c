#include <stdio.h>
#include <locale.h>
#include "utils.h"
#include "minheap.h"
#include "huffman_tree.h"
#include "dictionary.h"
#include "wchar.h"

void serial_comp(char* dir_path);

int main(){

    setlocale(LC_ALL, "");

    serial_comp("/home/marco/Descargas/textos");
    
    return 0;
}