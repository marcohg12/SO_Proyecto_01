gcc `pkg-config --cflags glib-2.0` `pkg-config --libs glib-2.0` main.c serial_comp.c thread_comp.c fork_comp.c utils.c node.c minheap.c huffman_tree.c dictionary.c -o compressor
./compressor