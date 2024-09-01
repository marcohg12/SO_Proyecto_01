// -----------------------------------------------------------------------------
// Este archivo contiene funciones auxiliares y de uso general
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <glib.h>

void print_char_frequencies(gpointer key, gpointer value, gpointer user_data) {
    g_print("%s: %d\n", (gchar*)key, GPOINTER_TO_INT(value));
}

GHashTable* create_word_freq_dict(char* file_path) {

    GHashTable* dict = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

    FILE* file = fopen(file_path, "r");

    // Obtenemos el largo del archivo
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    // Creamos un buffer para mantener el contenido del archivo
    char* buffer = (char*)malloc(file_size + 1);

    // Colocamos el contenido del archivo en el buffer
    size_t bytes_read = fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';

    // Cerramos el archivo
    fclose(file);

    char* ptr = buffer;
    char* end = buffer + bytes_read;

    // Recorremos el buffer
    while (ptr < end) {

        // Obtenemos el largo del siguiente caracter
        gchar* char_utf8 = g_utf8_next_char(ptr);
        size_t char_len = char_utf8 - ptr;

        // Obtenemos una copia del caracter
        gchar* char_copy = g_strndup(ptr, char_len);

        // Obtenemos la frecuencia del caracter en el diccionario
        gpointer count = g_hash_table_lookup(dict, char_copy);

        // Insertamos o actualizamos la frecuencia del caracter en el diccionario
        if (count > 0) {
            g_hash_table_replace(dict, char_copy, GUINT_TO_POINTER(GPOINTER_TO_UINT(count) + 1));
        }
        else {
            g_hash_table_insert(dict, char_copy, GUINT_TO_POINTER(1));
        }

        // Nos movemos al siguiente caracter
        ptr += char_len;
    }

    g_hash_table_foreach(dict, (GHFunc)print_char_frequencies, NULL);
    free(buffer);

    return dict;
}



