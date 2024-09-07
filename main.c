#include <stdio.h>
#include <locale.h>
#include "utils.h"
#include "minheap.h"
#include "huffman_tree.h"
#include "dictionary.h"
#include "wchar.h"

//void parallel_comp(char* dir_path);
void parallel_decomp(char* dir_path);

int main(){

    setlocale(LC_ALL, "");

    parallel_decomp("/home/arianaalvarado/Descargas/Proyecto1_SO/SO_Proyecto_01/parallel_comp_temp");
    
    
    return 0;
}