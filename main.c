#include <stdio.h>
#include <locale.h>
#include "utils.h"
#include "minheap.h"
#include "huffman_tree.h"
#include "dictionary.h"
#include "wchar.h"

int main(){

    setlocale(LC_ALL, "");

    Dictionary* dict = create_word_freq_dict("/home/marco/Descargas/pg135.txt");
    MinHeap* heap = get_min_heap_from_dict(dict);
    destroy_dictionary(dict);

    Node* root = create_huffman_tree(heap);

    destroy_min_heap(heap);
    
    char* code = get_huffman_code(root, (wchar_t)'@');
    wprintf(L"Code: %s\n", code);
    
    char* code2 = get_huffman_code(root, (wchar_t)'+');
    wprintf(L"Code: %s\n", code2);

    print_huffman_tree(root);

    destroy_huffman_tree(root);
    free(code);
    free(code2);
    
    return 0;
}