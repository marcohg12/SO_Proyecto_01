#include <stdio.h>
#include <locale.h>
#include "utils.h"
#include "minheap.h"
#include "huffman_tree.h"
#include "dictionary.h"
#include "wchar.h"

void serial_comp(char* dir_path);
void serial_decomp(char* dir_path);

int main(){

    setlocale(LC_ALL, "");

    serial_comp("/home/marco/Descargas/textos");
    //serial_decomp("/home/marco/Descargas/comp_temp");
    
    return 0;
}