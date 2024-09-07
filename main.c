#include <stdio.h>
#include <locale.h>
#include "utils.h"
#include "minheap.h"
#include "huffman_tree.h"
#include "dictionary.h"
#include "wchar.h"

void parallel_comp(char* dir_path);

int main(){

    setlocale(LC_ALL, "");

    parallel_comp("/home/arianaalvarado/Descargas/textos");
    
    
    return 0;
}