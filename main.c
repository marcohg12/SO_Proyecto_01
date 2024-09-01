#include <stdio.h>
#include <locale.h>
#include "utils.h"

int main(){

    setlocale(LC_ALL, "");
    create_word_freq_dict("/home/marco/Descargas/pg135.txt");
    return 0;
}