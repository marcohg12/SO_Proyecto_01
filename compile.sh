gcc compressor.c serial_comp.c thread_comp.c fork_comp.c utils.c node.c minheap.c huffman_tree.c dictionary.c code_list.c -o compressor -lrt
gcc decompressor.c serial_comp.c thread_comp.c fork_comp.c utils.c node.c minheap.c huffman_tree.c dictionary.c code_list.c -o decompressor -lrt
